#ifndef CHATLISTMODEL_H
#define CHATLISTMODEL_H

#include <QAbstractListModel>
#include "chat.h"

class ChatListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Chat *currentChat READ currentChat WRITE setCurrentChat NOTIFY currentChatChanged)

public:
    explicit ChatListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        if (m_chats.isEmpty())
            addChat();
    }

    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return m_chats.size();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_chats.size())
            return QVariant();

        const Chat *item = m_chats.at(index.row());
        switch (role) {
            case IdRole:
                return item->id();
            case NameRole:
                return item->name();
        }

        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override
    {
        QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        roles[NameRole] = "name";
        return roles;
    }

    Q_INVOKABLE Chat* addChat()
    {
        Chat *newChat = new Chat(this);
        beginInsertRows(QModelIndex(), m_chats.size(), m_chats.size());
        m_chats.append(newChat);
        endInsertRows();
        emit countChanged();
        setCurrentChat(newChat);
        return newChat;
    }

    Q_INVOKABLE void removeChat(Chat* chat)
    {
        if (!m_chats.contains(chat)) {
            qDebug() << "WARNING: Removing chat failed with id" << chat->id();
            return;
        }

        const bool chatIsCurrent = chat == m_currentChat;
        emit disconnectChat(chat);
        const int index = m_chats.indexOf(chat);
        beginRemoveRows(QModelIndex(), index, index);
        m_chats.removeAll(chat);
        endRemoveRows();
        delete chat;
        if (m_chats.isEmpty())
            addChat();
        else
            setCurrentChat(m_chats.first());
    }

    Chat *currentChat() const
    {
        return m_currentChat;
    }

    void setCurrentChat(Chat *chat)
    {
        if (!m_chats.contains(chat)) {
            qDebug() << "ERROR: Setting current chat failed with id" << chat->id();
            return;
        }

        if (m_currentChat) {
            Q_ASSERT(m_currentChat);
            emit disconnect(m_currentChat);
        }

        emit connectChat(chat);
        m_currentChat = chat;
        emit currentChatChanged();
    }

    Q_INVOKABLE Chat* get(int index)
    {
        if (index < 0 || index >= m_chats.size()) return nullptr;
        return m_chats.at(index);
    }


    int count() const { return m_chats.size(); }

Q_SIGNALS:
    void countChanged();
    void connectChat(Chat *);
    void disconnectChat(Chat *);
    void currentChatChanged();

private:
    Chat* m_currentChat;
    QList<Chat*> m_chats;
};

#endif // CHATITEMMODEL_H
