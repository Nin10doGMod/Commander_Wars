#ifndef DIALOGPASSWORDANDADRESS_H
#define DIALOGPASSWORDANDADRESS_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/textbox.h"
#include "objects/base/passwordbox.h"

class DialogPasswordAndAdress;
using spDialogPasswordAndAdress = oxygine::intrusive_ptr<DialogPasswordAndAdress>;

class DialogPasswordAndAdress final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogPasswordAndAdress(QString text);
   virtual ~DialogPasswordAndAdress() = default;

signals:
    void sigTextChanged(QString text, QString password);
    void sigCancel();
    void sigFinished();
private slots:
    void remove();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    spTextbox m_pTextbox;
    spPasswordbox m_pPasswordbox;
};

#endif // DIALOGPASSWORDANDADRESS_H
