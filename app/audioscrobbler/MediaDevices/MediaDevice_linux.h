/*
   Copyright 2005-2010 Last.fm Ltd.
      - Primarily authored by Max Howell, Jono Cole and Doug Mansell

   This file is part of the Last.fm Desktop Application Suite.

   lastfm-desktop is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   lastfm-desktop is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with lastfm-desktop.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MEDIA_DEVICE_H
#define MEDIA_DEVICE_H

#include <QObject>
#include <QSqlDatabase>
#include <lastfm/Track>

class MediaDevice: public QObject
{
    Q_OBJECT
public:
    MediaDevice();
    ~MediaDevice();
    
    QSqlDatabase database() const;

    void setMountPath( const QString& path ){ m_mountPath = path; }

    QString error() const { return m_error; }

    virtual QString tableName() const = 0;
    virtual QList<Track> tracksToScrobble() = 0;

protected:
    QString m_error;
    QString m_mountPath;
};


#endif // MEDIA_DEVICE_H
