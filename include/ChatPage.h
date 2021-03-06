/*
 * nheko Copyright (C) 2017  Konstantinos Sideris <siderisk@auth.gr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QPixmap>
#include <QTimer>
#include <QWidget>

#include "MatrixClient.h"
#include "RoomInfo.h"
#include "RoomList.h"
#include "TextInputWidget.h"
#include "TimelineViewManager.h"
#include "TopRoomBar.h"
#include "UserInfoWidget.h"

namespace Ui
{
class ChatPage;
}

class ChatPage : public QWidget
{
	Q_OBJECT

public:
	ChatPage(QSharedPointer<MatrixClient> client, QWidget *parent = 0);
	~ChatPage();

	// Initialize all the components of the UI.
	void bootstrap(QString userid, QString homeserver, QString token);

signals:
	void close();
	void changeWindowTitle(const QString &msg);

private slots:
	void showUnreadMessageNotification(int count);
	void updateTopBarAvatar(const QString &roomid, const QPixmap &img);
	void updateOwnProfileInfo(const QUrl &avatar_url, const QString &display_name);
	void setOwnAvatar(const QPixmap &img);
	void initialSyncCompleted(const SyncResponse &response);
	void syncCompleted(const SyncResponse &response);
	void syncFailed(const QString &msg);
	void changeTopRoomInfo(const RoomInfo &info);
	void startSync();
	void logout();

private:
	Ui::ChatPage *ui;

	RoomList *room_list_;
	TimelineViewManager *view_manager_;

	TopRoomBar *top_bar_;
	TextInputWidget *text_input_;

	QTimer *sync_timer_;
	int sync_interval_;

	RoomInfo current_room_;
	QMap<QString, QPixmap> room_avatars_;

	UserInfoWidget *user_info_widget_;

	// Matrix Client API provider.
	QSharedPointer<MatrixClient> client_;
};

#endif  // CHATPAGE_H
