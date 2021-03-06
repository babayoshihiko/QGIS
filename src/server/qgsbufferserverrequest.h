/***************************************************************************
                          qgsbufferserverrequest.h

  Define response wrapper for storing responsea in buffer
  -------------------
  begin                : 2017-01-03
  copyright            : (C) 2017 by David Marteau
  email                : david dot marteau at 3liz dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSBUFFERSERVERREQUEST_H
#define QGSBUFFERSERVERREQUEST_H

#include "qgsserverrequest.h"

#include <QBuffer>
#include <QByteArray>
#include <QMap>

/**
 * \ingroup server
 * QgsBufferServerRequest
 * Class defining request with  data
 */
class QgsBufferServerRequest : public QgsServerRequest
{
  public:

    /**
    * Constructor
    *
    * \param url the url string
    * \param method the request method
    */
    QgsBufferServerRequest( const QString &url, Method method = GetMethod, QByteArray *data = nullptr );

    /**
     * Constructor
     *
     * \param url QUrl
     * \param method the request method
     */
    QgsBufferServerRequest( const QUrl &url, Method method = GetMethod, QByteArray *data = nullptr );

    ~QgsBufferServerRequest();

    virtual QByteArray data() const { return mData; }

  private:
    QByteArray mData;
};

#endif
