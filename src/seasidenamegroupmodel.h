/*
 * Copyright (C) 2013 Jolla Mobile <bea.lam@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef SEASIDEPEOPLENAMEGROUPMODEL_H
#define SEASIDEPEOPLENAMEGROUPMODEL_H

#include <seasidecache.h>
#include <seasidefilteredmodel.h>

#include <QAbstractListModel>
#include <QStringList>

#include <QContactId>

QTCONTACTS_USE_NAMESPACE

class SeasideNameGroup
{
public:
    SeasideNameGroup() : count(0) {}
    SeasideNameGroup(const QString &n, const QSet<quint32> &ids = QSet<quint32>(), int c = -1)
        : name(n), count(c), contactIds(ids)
    {
        if (count == -1) {
            count = contactIds.count();
        }
    }

    inline bool operator==(const SeasideNameGroup &other) { return other.name == name; }

    QString name;
    int count;
    QSet<quint32> contactIds;
};

class SeasideNameGroupModel : public QAbstractListModel, public SeasideNameGroupChangeListener
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(int requiredProperty READ requiredProperty WRITE setRequiredProperty NOTIFY requiredPropertyChanged)
    Q_ENUMS(RequiredPropertyType)
public:
    enum Role {
        NameRole = Qt::UserRole,
        EntryCount
    };

    enum RequiredPropertyType {
        NoPropertyRequired = SeasideFilteredModel::NoPropertyRequired,
        AccountUriRequired = SeasideFilteredModel::AccountUriRequired,
        PhoneNumberRequired = SeasideFilteredModel::PhoneNumberRequired,
        EmailAddressRequired = SeasideFilteredModel::EmailAddressRequired
    };

    SeasideNameGroupModel(QObject *parent = 0);
    ~SeasideNameGroupModel();

    int requiredProperty() const;
    void setRequiredProperty(int type);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    void nameGroupsUpdated(const QHash<QString, QSet<quint32> > &groups);

    virtual QHash<int, QByteArray> roleNames() const;

signals:
    void countChanged();
    void requiredPropertyChanged();

private:
    int countFilteredContacts(const QSet<quint32> &contactIds) const;

    QList<SeasideNameGroup> m_groups;
    int m_requiredProperty;
};

#endif
