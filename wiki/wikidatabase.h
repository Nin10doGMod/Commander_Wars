#ifndef WIKIDATABASE_H
#define WIKIDATABASE_H

#include <QObject>
#include <QVector>

#include "resource_management/ressourcemanagement.h"
#include "wiki/wikipage.h"


class WikiDatabase;
using spWikiDatabase = oxygine::intrusive_ptr<WikiDatabase>;

/**
 * @brief The WikiDatabase class
 */
class WikiDatabase : public QObject, public RessourceManagement<WikiDatabase>
{
    Q_OBJECT
public:
    virtual ~WikiDatabase() = default;
    /**
     * @brief m_Entries
     * 0 = Name
     * 1 = ID
     * 2 = Tags
     */
    using pageData = std::tuple<QString, QString, QStringList> ;
    /**
     * @brief load
     */
    void load();
    /**
     * @brief getPage
     * @param data
     * @return
     */
    spWikipage getPage(pageData data);
    /**
     * @brief getEntries
     * @param searchTerm
     * @return
     */
    QVector<pageData> getEntries(QString searchTerm, bool onlyTag);
    /**
     * @brief getEntries
     * @param entry
     * @return
     */
    pageData getEntry(qint32 entry);
    /**
     * @brief hasEntry
     * @param file1
     * @return
     */
    bool hasEntry(QString file1);
    /**
     * @brief getEntry
     * @param id
     * @return
     */
    WikiDatabase::pageData getEntry(QString id);
    /**
     * @brief tagMatches
     * @param tags
     * @param searchTerm
     * @return
     */
    bool tagMatches(QStringList tags, QString searchTerm);
    /**
     * @brief getTags
     * @return
     */
    QVector<QString> getTags();
    /**
     * @brief getIcon
     * @param file
     * @return
     */
    oxygine::spSprite getIcon(QString file, qint32 size);
signals:

public slots:

private:
    friend RessourceManagement<WikiDatabase>;
    friend class oxygine::intrusive_ptr<WikiDatabase>;
    explicit WikiDatabase();

private:
    QVector<pageData> m_Entries;
};

#endif // WIKIDATABASE_H
