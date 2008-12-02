/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.2, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/

#include "formclasswizardpage.h"
#include "ui_formclasswizardpage.h"
#include "formclasswizardparameters.h"

#include <coreplugin/icore.h>
#include <cppeditor/cppeditorconstants.h>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include <QtGui/QMessageBox>
#include <QtGui/QAbstractButton>

static const char *formClassWizardPageGroupC = "FormClassWizardPage";
static const char *translationKeyC = "RetranslationSupport";
static const char *embeddingModeKeyC = "Embedding";

namespace Designer {
namespace Internal {

// ----------------- FormClassWizardPage

FormClassWizardPage::FormClassWizardPage(QWidget * parent) :
    QWizardPage(parent),
    m_ui(new Ui::FormClassWizardPage),
    m_isValid(false)
{
    m_ui->setupUi(this);

    m_ui->newClassWidget->setBaseClassInputVisible(false);
    m_ui->newClassWidget->setNamespacesEnabled(true);

    connect(m_ui->newClassWidget, SIGNAL(validChanged()), this, SLOT(slotValidChanged()));

    m_ui->extensionWidget->setVisible(false);
    connect(m_ui->moreButton, SIGNAL(clicked(bool)), m_ui->extensionWidget, SLOT(setVisible(bool)));

    restoreSettings();
}

FormClassWizardPage::~FormClassWizardPage()
{
    delete m_ui;
}

void FormClassWizardPage::setSuffixes(const QString &header, const QString &source,  const QString &form)
{
    m_ui->newClassWidget->setSourceExtension(source);
    m_ui->newClassWidget->setHeaderExtension(header);
    m_ui->newClassWidget->setFormExtension(form);
}

void FormClassWizardPage::setClassName(const QString &suggestedClassName)
{
    // Is it valid, now?
    m_ui->newClassWidget->setClassName(suggestedClassName);
    slotValidChanged();
}

int FormClassWizardPage::uiClassEmbedding() const
{
    if (m_ui->ptrAggregationRadioButton->isChecked())
        return PointerAggregatedUiClass;
    if (m_ui->aggregationButton->isChecked())
        return AggregatedUiClass;
    return InheritedUiClass;
}

void FormClassWizardPage::setUiClassEmbedding(int v)
{
    switch (v) {
    case PointerAggregatedUiClass:
        m_ui->ptrAggregationRadioButton->setChecked(true);
        break;
    case AggregatedUiClass:
        m_ui->aggregationButton->setChecked(true);
        break;
    case InheritedUiClass:
        m_ui->multipleInheritanceButton->setChecked(true);
        break;
    }
}

bool FormClassWizardPage::hasRetranslationSupport() const
{
    return m_ui->retranslateCheckBox->isChecked();
}

void FormClassWizardPage::setRetranslationSupport(bool v)
{
    m_ui->retranslateCheckBox->setChecked(v);
}

QString FormClassWizardPage::path() const
{
    return m_ui->newClassWidget->path();
}

void FormClassWizardPage::setPath(const QString &p)
{
    m_ui->newClassWidget->setPath(p);
}

void FormClassWizardPage::getParameters(FormClassWizardParameters *p) const
{
    p->embedding = static_cast<UiClassEmbedding>(uiClassEmbedding());
    p->languageChange = m_ui->retranslateCheckBox->isChecked();
    p->className = m_ui->newClassWidget->className();
    p->path = path();
    p->sourceFile = m_ui->newClassWidget->sourceFileName();
    p->headerFile = m_ui->newClassWidget->headerFileName();
    p->uiFile = m_ui->newClassWidget-> formFileName();
}

void FormClassWizardPage::slotValidChanged()
{
    const bool validNow = m_ui->newClassWidget->isValid();
    if (m_isValid != validNow) {
        m_isValid = validNow;
        emit completeChanged();
    }
}

bool FormClassWizardPage::isComplete() const
{
    return m_isValid;
}

bool FormClassWizardPage::validatePage()
{
    QString errorMessage;
    const bool rc = m_ui->newClassWidget->isValid(&errorMessage);
    if (rc) {
        saveSettings();
    }  else {
        QMessageBox::critical(this, tr("%1 - Error").arg(title()), errorMessage);
    }
    return rc;
}

void FormClassWizardPage::saveSettings()
{
    Core::ICore *core = ExtensionSystem::PluginManager::instance()->getObject<Core::ICore>();
    if (QSettings *settings = core->settings()) {
        settings->beginGroup(QLatin1String(formClassWizardPageGroupC));
        settings->setValue(QLatin1String(translationKeyC), hasRetranslationSupport());
        settings->setValue(QLatin1String(embeddingModeKeyC), uiClassEmbedding());
        settings->endGroup();
    }
}

void FormClassWizardPage::restoreSettings()
{
    bool retranslationSupport = true;
    int embedding =  PointerAggregatedUiClass;

    Core::ICore *core = ExtensionSystem::PluginManager::instance()->getObject<Core::ICore>();
    if (QSettings *settings = core->settings()) {

        QString key = QLatin1String(formClassWizardPageGroupC);
        key += QLatin1Char('/');
        const int groupLength = key.size();

        key += QLatin1String(translationKeyC);
        retranslationSupport = settings->value(key, retranslationSupport).toBool();

        key.truncate(groupLength);
        key += QLatin1String(embeddingModeKeyC);
        embedding =  settings->value(key, embedding).toInt();
    }
    setUiClassEmbedding(embedding);
    setRetranslationSupport(retranslationSupport);
}

} // namespace Internal
} // namespace Designer
