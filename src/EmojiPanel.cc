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

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "Avatar.h"
#include "EmojiPanel.h"
#include "FlatButton.h"

EmojiPanel::EmojiPanel(QWidget *parent)
    : QFrame(parent)
{
	setStyleSheet("background-color: #f8f8f8;");

	setParent(0);  // Create TopLevel-Widget
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);

	// TODO: Make it MainWindow aware
	auto main_frame_ = new QFrame(this);
	main_frame_->setMinimumSize(370, 350);
	main_frame_->setMaximumSize(370, 350);

	auto top_layout = new QVBoxLayout(this);
	top_layout->addWidget(main_frame_);
	top_layout->setMargin(0);

	auto content_layout = new QVBoxLayout();
	content_layout->setMargin(0);
	main_frame_->setLayout(content_layout);

	auto emoji_categories = new QFrame(main_frame_);
	emoji_categories->setStyleSheet("background-color: #eee");

	auto categories_layout = new QHBoxLayout();
	categories_layout->setSpacing(6);
	categories_layout->setMargin(5);

	auto category_icon_size = 20;

	auto people_category = new FlatButton(emoji_categories);
	people_category->setIcon(QIcon(":/icons/icons/emoji-categories/people.png"));
	people_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto nature_category = new FlatButton(emoji_categories);
	nature_category->setIcon(QIcon(":/icons/icons/emoji-categories/nature.png"));
	nature_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto food_category = new FlatButton(emoji_categories);
	food_category->setIcon(QIcon(":/icons/icons/emoji-categories/foods.png"));
	food_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto activity_category = new FlatButton(emoji_categories);
	activity_category->setIcon(QIcon(":/icons/icons/emoji-categories/activity.png"));
	activity_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto travel_category = new FlatButton(emoji_categories);
	travel_category->setIcon(QIcon(":/icons/icons/emoji-categories/travel.png"));
	travel_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto objects_category = new FlatButton(emoji_categories);
	objects_category->setIcon(QIcon(":/icons/icons/emoji-categories/objects.png"));
	objects_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto symbols_category = new FlatButton(emoji_categories);
	symbols_category->setIcon(QIcon(":/icons/icons/emoji-categories/symbols.png"));
	symbols_category->setIconSize(QSize(category_icon_size, category_icon_size));

	auto flags_category = new FlatButton(emoji_categories);
	flags_category->setIcon(QIcon(":/icons/icons/emoji-categories/flags.png"));
	flags_category->setIconSize(QSize(category_icon_size, category_icon_size));

	categories_layout->addWidget(people_category);
	categories_layout->addWidget(nature_category);
	categories_layout->addWidget(food_category);
	categories_layout->addWidget(activity_category);
	categories_layout->addWidget(travel_category);
	categories_layout->addWidget(objects_category);
	categories_layout->addWidget(symbols_category);
	categories_layout->addWidget(flags_category);

	emoji_categories->setLayout(categories_layout);

	content_layout->addStretch(1);
	content_layout->addWidget(emoji_categories);

	setLayout(top_layout);

	// TODO: Add parallel animation with geometry
	opacity_ = new QGraphicsOpacityEffect(this);
	opacity_->setOpacity(1.0);

	setGraphicsEffect(opacity_);

	animation_ = new QPropertyAnimation(opacity_, "opacity", this);
	animation_->setDuration(100);
	animation_->setStartValue(1.0);
	animation_->setEndValue(0.0);

	connect(animation_, &QPropertyAnimation::finished, [this]() {
		if (animation_->direction() == QAbstractAnimation::Forward) {
			this->close();
			this->deleteLater();
		}
	});
}

void EmojiPanel::leaveEvent(QEvent *event)
{
	Q_UNUSED(event);

	fadeOut();
}

void EmojiPanel::fadeOut()
{
	animation_->setDirection(QAbstractAnimation::Forward);
	animation_->start();
}

void EmojiPanel::fadeIn()
{
	animation_->setDirection(QAbstractAnimation::Backward);
	animation_->start();
}
