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

#include <QBrush>
#include <QDebug>
#include <QDesktopServices>
#include <QImage>
#include <QPainter>
#include <QPixmap>

#include "ImageItem.h"
#include "ImageOverlayDialog.h"

ImageItem::ImageItem(QSharedPointer<MatrixClient> client, const Event &event, const QString &body, const QUrl &url, QWidget *parent)
    : QWidget(parent)
    , url_{url}
    , text_{body}
    , event_{event}
    , client_{client}
{
	setMaximumSize(max_width_, max_height_);
	setMouseTracking(true);
	setCursor(Qt::PointingHandCursor);
	setAttribute(Qt::WA_Hover, true);

	QList<QString> url_parts = url_.toString().split("mxc://");

	if (url_parts.size() != 2) {
		qDebug() << "Invalid format for image" << url_.toString();
		return;
	}

	QString media_params = url_parts[1];
	url_ = QString("%1/_matrix/media/r0/download/%2").arg(client_.data()->getHomeServer(), media_params);

	client_.data()->downloadImage(event.eventId(), url_);

	connect(client_.data(),
		SIGNAL(imageDownloaded(const QString &, const QPixmap &)),
		this,
		SLOT(imageDownloaded(const QString &, const QPixmap &)));
}

void ImageItem::imageDownloaded(const QString &event_id, const QPixmap &img)
{
	if (event_id != event_.eventId())
		return;

	setImage(img);
}

void ImageItem::openUrl()
{
	if (url_.toString().isEmpty())
		return;

	if (!QDesktopServices::openUrl(url_))
		qWarning() << "Could not open url" << url_.toString();
}

void ImageItem::scaleImage()
{
	if (image_.isNull())
		return;

	auto width_ratio = (double)max_width_ / (double)image_.width();
	auto height_ratio = (double)max_height_ / (double)image_.height();

	auto min_aspect_ratio = std::min(width_ratio, height_ratio);

	if (min_aspect_ratio > 1) {
		width_ = image_.width();
		height_ = image_.height();
	} else {
		width_ = image_.width() * min_aspect_ratio;
		height_ = image_.height() * min_aspect_ratio;
	}

	setMinimumSize(width_, height_);
	scaled_image_ = image_.scaled(width_, height_, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QSize ImageItem::sizeHint() const
{
	if (image_.isNull())
		return QSize(max_width_, bottom_height_);

	return QSize(width_, height_);
}

void ImageItem::setImage(const QPixmap &image)
{
	image_ = image;
	scaleImage();
	update();
}

void ImageItem::mousePressEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton)
		return;

	if (image_.isNull()) {
		openUrl();
		return;
	}

	auto point = event->pos();

	// Click on the text box.
	if (QRect(0, height_ - bottom_height_, width_, bottom_height_).contains(point)) {
		openUrl();
	} else {
		auto image_dialog = new ImageOverlayDialog(image_, this);
		image_dialog->show();
	}
}

void ImageItem::resizeEvent(QResizeEvent *event)
{
	Q_UNUSED(event);

	scaleImage();
}

void ImageItem::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QFont font("Open Sans");
	font.setPixelSize(12);

	QFontMetrics metrics(font);
	int fontHeight = metrics.height();

	if (image_.isNull()) {
		int height = fontHeight + 10;

		setMinimumSize(max_width_, fontHeight + 10);

		QString elidedText = metrics.elidedText(text_, Qt::ElideRight, max_width_ - 10);

		painter.setFont(font);
		painter.setPen(QPen(QColor(66, 133, 244)));
		painter.drawText(QPoint(0, height / 2 + 2), elidedText);

		return;
	}

	painter.fillRect(QRect(0, 0, width_, height_), scaled_image_);

	if (underMouse()) {
		// Bottom text section
		painter.fillRect(QRect(0, height_ - bottom_height_, width_, bottom_height_),
				 QBrush(QColor(33, 33, 33, 128)));

		QString elidedText = metrics.elidedText(text_, Qt::ElideRight, width_ - 10);

		font.setWeight(500);
		painter.setFont(font);
		painter.setPen(QPen(QColor("white")));
		painter.drawText(QPoint(5, height_ - fontHeight / 2), elidedText);
	}
}
