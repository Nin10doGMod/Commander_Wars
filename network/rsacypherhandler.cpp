#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

#include <QRandomGenerator>
#include <QJsonObject>

#include "network/rsacypherhandler.h"
#include "network/JsonKeys.h"

#include "coreengine/gameconsole.h"

RsaCypherHandler::RsaCypherHandler()
    : m_privateKey(EVP_PKEY_new()),
      m_publicKey(EVP_PKEY_new()),
      CYPHER(EVP_aes_256_ecb())
{
    ERR_load_CRYPTO_strings();
    seedRng();
    generateKeys();
}

RsaCypherHandler::~RsaCypherHandler()
{
    EVP_PKEY_free(m_privateKey);
    EVP_PKEY_free(m_publicKey);
}

void RsaCypherHandler::generateKeys()
{
    BIGNUM* bigNumber = BN_new();
    BN_set_word(bigNumber, RSA_F4);
    static constexpr qint32 bits = 2048;

    //here we generate the RSA keys
    RSA* rsaKey = RSA_new();
    RSA_generate_key_ex(rsaKey, bits, bigNumber, nullptr);

    // store data in memory bio
    BIO * publicBio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey (publicBio, rsaKey);
    BIO * privateBio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(privateBio, rsaKey, nullptr, nullptr, 0, nullptr, nullptr);

    // reserve key strings
    auto privateLength = BIO_pending(privateBio);
    auto publicLength = BIO_pending(publicBio);
    std::vector<char> privateKeyVector(privateLength + 1);
    std::vector<char> publicKeyVector(publicLength + 1);
    //now we read the BIO into a buffer
    BIO_read(privateBio, &privateKeyVector[0], privateLength);
    BIO_read(publicBio, &publicKeyVector[0], publicLength);
    privateKeyVector[privateLength] = '\0';
    publicKeyVector[publicLength] = '\0';
    // receive rsa ptr
    BIO * publicKeyBio = BIO_new_mem_buf(static_cast<void*>(&publicKeyVector[0]), publicLength);
    BIO * privateKeyBio = BIO_new_mem_buf(static_cast<void*>(&privateKeyVector[0]), privateLength);
    RSA* privateRsa = PEM_read_bio_RSAPrivateKey(privateKeyBio, nullptr, nullptr, nullptr);
    RSA* publicRsa = PEM_read_bio_RSAPublicKey(publicKeyBio, nullptr, nullptr, nullptr);
    EVP_PKEY_assign_RSA(m_privateKey, privateRsa);
    EVP_PKEY_assign_RSA(m_publicKey, publicRsa);
    m_publicKeyStr = QString(&publicKeyVector[0]);

    // clean up
    BIO_free_all(publicBio);
    BIO_free_all(privateBio);
    BIO_free(privateKeyBio);
    BIO_free(publicKeyBio);
    BN_free(bigNumber);
    RSA_free(rsaKey);
}

QString RsaCypherHandler::getPublicKey() const
{
    return m_publicKeyStr;
}

void RsaCypherHandler::seedRng() const
{
    QRandomGenerator rng = QRandomGenerator::securelySeeded();
    constexpr int size = 64;
    quint8 buf[size];
    for (qint32 i = 0; i < size; ++i)
    {
        buf[i] = rng.bounded(0, 256);
    }
    RAND_seed(buf, size);
}

bool RsaCypherHandler::getReady() const
{
    return m_ready;
}

bool RsaCypherHandler::encryptRSA(const QString & publicKey, const QByteArray & message, QByteArray & encryptedKey, QByteArray & encrpytedMessage, QByteArray & iv)
{
    // setup openssl data
    // read public key
    auto stdPublicKey = publicKey.toStdString();
    size_t keyLength = stdPublicKey.length();
    bool success = false;
    EVP_PKEY* evpKey = EVP_PKEY_new();
    BIO* publicKeyBio = BIO_new_mem_buf(static_cast<const void*>(stdPublicKey.c_str()), keyLength);
    RSA* publicRsa = PEM_read_bio_RSAPublicKey(publicKeyBio, nullptr, nullptr, nullptr);
    EVP_PKEY_assign_RSA(evpKey, publicRsa);
    if (evpKey != nullptr)
    {
        qint32 blockLength = 0;
        qint32 encryptedMessageLength = 0;
        qint32 encryptedKeyLength = 0;
        std::vector<unsigned char> internalEncryptedKey(EVP_PKEY_size(evpKey));
        auto* encryptedKeyPtr = &internalEncryptedKey[0];
        qint32 ivSize = EVP_CIPHER_iv_length(CYPHER);
        std::vector<unsigned char> internalIv(ivSize);
        EVP_CIPHER_CTX* rsaEncryptContext = EVP_CIPHER_CTX_new();
        if(EVP_SealInit(rsaEncryptContext, CYPHER, &encryptedKeyPtr, &encryptedKeyLength, &internalIv[0], &evpKey, 1) > 0) // internalIv
        {
            const unsigned char* inData = reinterpret_cast<const unsigned char*>(message.constData());
            int blocksize = EVP_CIPHER_CTX_block_size(rsaEncryptContext);
            qint32 inLength = message.length();
            std::vector<unsigned char> internalEncryptedMessage(inLength + blocksize + 1);
            blockLength = internalEncryptedMessage.size();
            if(EVP_SealUpdate(rsaEncryptContext, &internalEncryptedMessage[0] + encryptedMessageLength, &blockLength, inData, inLength) > 0)
            {
                encryptedMessageLength = blockLength;
                if(EVP_SealFinal(rsaEncryptContext, &internalEncryptedMessage[0] + blockLength, &blockLength) > 0)
                {
                    encryptedMessageLength += blockLength;
                    internalEncryptedMessage.resize(encryptedMessageLength);
                    encryptedKey = QByteArray(reinterpret_cast<char*>(&internalEncryptedKey[0]), encryptedKeyLength);
                    encrpytedMessage = QByteArray(reinterpret_cast<char*>(&internalEncryptedMessage[0]), encryptedMessageLength);
                    iv = QByteArray(reinterpret_cast<char*>(&internalIv[0]), ivSize);
                    success = true;
                }
            }
        }
        EVP_CIPHER_CTX_free(rsaEncryptContext);
    }
    BIO_free_all(publicKeyBio);
    EVP_PKEY_free(evpKey);
    if(!success)
    {
        printLastError();
    }
    return success;
}

void RsaCypherHandler::printLastError() const
{
    char errorMsg[512];
    ERR_error_string(ERR_get_error(), errorMsg);
    CONSOLE_PRINT(QString("Could not encyrpt data") + errorMsg, GameConsole::eLogLevels::eWARNING);
}

bool RsaCypherHandler::decryptRSA(const QByteArray & encryptedKey, const QByteArray & encrpytedMessage, const QByteArray & iv, QByteArray & decryptedMessage)
{
    bool success = false;

    // Allocate memory for everything
    qint32 encryptedKeySize = encryptedKey.length();
    EVP_CIPHER_CTX* rsaDecryptContext = EVP_CIPHER_CTX_new();
    if(EVP_OpenInit(rsaDecryptContext, CYPHER, reinterpret_cast<const unsigned char*>(encryptedKey.constData()), encryptedKeySize,
                    reinterpret_cast<const unsigned char*>(iv.constData()), m_privateKey) > 0)
    {
        qint32 encryptedMessageLength = encrpytedMessage.length();
        std::vector<unsigned char> internalDecryptedMessage(encryptedMessageLength);
        qint32 blockLength = 0;
        if(EVP_OpenUpdate(rsaDecryptContext, &internalDecryptedMessage[0], &blockLength, reinterpret_cast<const unsigned char*>(encrpytedMessage.constData()), encryptedMessageLength) > 0)
        {
            qint32 decryptedMessageLength = blockLength;
            if(EVP_OpenFinal(rsaDecryptContext, &internalDecryptedMessage[0] + blockLength, &blockLength) > 0)
            {
                decryptedMessageLength += blockLength;
                internalDecryptedMessage.resize(decryptedMessageLength);
                decryptedMessage = QByteArray(reinterpret_cast<const char*>(&internalDecryptedMessage[0]), decryptedMessageLength);
                success = true;
            }
        }
    }
    EVP_CIPHER_CTX_free(rsaDecryptContext);
    if(!success)
    {
        printLastError();
    }
    return success;
}

QByteArray RsaCypherHandler::toByteArray(const QJsonArray & jsonArray)
{
    QByteArray array;
    for (auto & item : jsonArray)
    {
        array.append(static_cast<char>(item.toInt()));
    }
    return array;
}

QJsonArray RsaCypherHandler::toJsonArray(const QByteArray & byteArray)
{
    QJsonArray array;
    for (qint32 i = 0; i < byteArray.size(); ++i)
    {
        array.append(byteArray[i]);
    }
    return array;
}

QByteArray RsaCypherHandler::getRequestKeyMessage(NetworkCommands::PublicKeyActions action) const
{
    QString command = NetworkCommands::REQUESTPUBLICKEY;
    CONSOLE_PRINT("Sending message: " + command, GameConsole::eLogLevels::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QJsonDocument doc(data);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray RsaCypherHandler::getPublicKeyMessage(NetworkCommands::PublicKeyActions action) const
{
    QString command = NetworkCommands::SENDPUBLICKEY;
    CONSOLE_PRINT("Sending message: " + command, GameConsole::eLogLevels::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_PUBLICKEY, m_publicKeyStr);
    data.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QJsonDocument doc(data);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray RsaCypherHandler::getDecryptedMessage(const QJsonDocument & encryptedMessage)
{
    QJsonObject objData = encryptedMessage.object();
    auto jsonMessage = objData.value(JsonKeys::JSONKEY_ENCRYPTEDMESSAGE).toArray();
    auto jsonEncryptedKey = objData.value(JsonKeys::JSONKEY_ENCRYPTEDKEY).toArray();
    auto jsonIv = objData.value(JsonKeys::JSONKEY_IV).toArray();
    QByteArray decryptedMessage;
    decryptRSA(toByteArray(jsonEncryptedKey), toByteArray(jsonMessage), toByteArray(jsonIv), decryptedMessage);
    return decryptedMessage;
}

QJsonDocument RsaCypherHandler::getEncryptedMessage(const QString & publicKey, NetworkCommands::PublicKeyActions action, const QByteArray & message)
{
    QJsonObject data;
    QString command = NetworkCommands::CRYPTEDMESSAGE;
    CONSOLE_PRINT("Sending message: " + command, GameConsole::eLogLevels::eDEBUG);
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QByteArray encryptedKey;
    QByteArray encrpytedMessage;
    QByteArray iv;
    encryptRSA(publicKey, message, encryptedKey, encrpytedMessage, iv);
    data.insert(JsonKeys::JSONKEY_ENCRYPTEDMESSAGE, toJsonArray(encrpytedMessage));
    data.insert(JsonKeys::JSONKEY_ENCRYPTEDKEY, toJsonArray(encryptedKey));
    data.insert(JsonKeys::JSONKEY_IV, toJsonArray(iv));
    QJsonDocument doc(data);
    return doc;
}
