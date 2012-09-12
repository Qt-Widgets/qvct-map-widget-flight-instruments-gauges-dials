// INDENTING (emacs/vi): -*- mode:c++; tab-width:2; c-basic-offset:2; intent-tabs-mode:nil; -*- ex: set tabstop=2 expandtab:

/*
 * Qt Virtual Chart Table (QVCT)
 * Copyright (C) 2012 Cedric Dufour <http://cedric.dufour.name>
 * Author: Cedric Dufour <http://cedric.dufour.name>
 *
 * The Qt Virtual Chart Table (QVCT) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, Version 3.
 *
 * The Qt Virtual Chart Table (QVCT) is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 */

// C/C++
#include <cmath>

// QT
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QRectF>
#include <QWidget>

// QVCT
#include "QVCT.hpp"
#include "QVCTRuntime.hpp"
#include "charts/CChart.hpp"
#include "charts/CChartGDAL.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CChart::CChart( QWidget* _pqParent, const QString& _rqsFileName )
  : QWidget( _pqParent )
  , poChartGDAL( 0 )
{
  poChartGDAL = new CChartGDAL( _rqsFileName );
  if( poChartGDAL->getStatus() != QVCT::OK ) return;
  setCursor( QCursor( QPixmap( ":cursors/crosshair.png" ), 15, 15 ) );
  qPointFDatPosition = QRectF( poChartGDAL->getDatGeometry() ).center();
  bPositionLock = true;
  fdZoom = 1.0;
  bZoomLock = true;
}

CChart::~CChart()
{
  if( poChartGDAL ) delete poChartGDAL;
}


//------------------------------------------------------------------------------
// METHODS: QWidget (OVERRIDE)
//------------------------------------------------------------------------------

void CChart::paintEvent( QPaintEvent* _pqPaintEvent )
{
  //qDebug( "DEBUG[%s]", Q_FUNC_INFO );
  qRectFDrawArea = QRectF( 0, 0, QWidget::width(), QWidget::height() );
  draw();
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

//
// SETTERS
//

void CChart::resetDrawArea()
{
  qRectFDrawArea = QRectF( 0, 0, QWidget::width(), QWidget::height() );
}

void CChart::setGeoPosition( const CDataPosition& _roGeoPosition )
{
  qPointFDatPosition = poChartGDAL->toDatPosition( _roGeoPosition );
}

//
// GETTERS
//

QVCT::EStatus CChart::getStatus() const
{
  return poChartGDAL->getStatus();
}

QString CChart::getFileName() const
{
  return poChartGDAL->getFileName();
}

CDataPosition CChart::getGeoPositionCenter() const
{
  return poChartGDAL->toGeoPosition( poChartGDAL->getDatGeometry().center() );
}

CDataPosition CChart::getGeoPosition() const
{
  return poChartGDAL->toGeoPosition( qPointFDatPosition );
}

QPointF CChart::getDrawPositionCenter() const
{
  return qRectFDrawArea.center();
}

double CChart::getZoomFit() const
{
  QRect __qRectDatGeometry = poChartGDAL->getDatGeometry();
  double __fdZoomWidth = qRectFDrawArea.width() / (double)__qRectDatGeometry.width();
  double __fdZoomHeight = qRectFDrawArea.height() / (double)__qRectDatGeometry.height();
  return( __fdZoomWidth < __fdZoomHeight ? __fdZoomWidth : __fdZoomHeight );
}

double CChart::getZoomArea( const CDataPosition& _roGeoPosition1, const CDataPosition& _roGeoPosition2 ) const
{
  double __fdLongitude1 = _roGeoPosition1.getLongitude(), __fdLongitude2 = _roGeoPosition2.getLongitude();
  double __fdLatitude1 = _roGeoPosition1.getLatitude(), __fdLatitude2 = _roGeoPosition2.getLatitude();
  CDataPosition __oGeoPosition( ( __fdLongitude1 + __fdLongitude2 ) / 2.0, ( __fdLatitude1 + __fdLatitude1 ) / 2.0 );
  double __fdResolution = poChartGDAL->getResolution( poChartGDAL->toDatPosition( __oGeoPosition ) );
  double __fdLatitudeMaxCos;
  if( __fdLatitude1 * __fdLatitude2 < 0 ) __fdLatitudeMaxCos = 0;
  else if( fabs( __fdLatitude1 ) < fabs( __fdLatitude2 ) ) __fdLatitudeMaxCos = __fdLatitude1;
  else __fdLatitudeMaxCos = __fdLatitude2;
  double __fdZoomWidth = qRectFDrawArea.width() / ( fabs( ( __fdLongitude2 - __fdLongitude1 ) * cos( __fdLatitudeMaxCos * 0.01745329252 ) ) * 111120.0 / __fdResolution );
  double __fdZoomHeight = qRectFDrawArea.height() / ( fabs( __fdLatitude2 - __fdLatitude1 ) * 111120.0 / __fdResolution );
  return( __fdZoomWidth < __fdZoomHeight ? __fdZoomWidth : __fdZoomHeight );
}

double CChart::getResolution() const
{
  return poChartGDAL->getResolution( qPointFDatPosition );
}

//
// OTHER
//

CDataPosition CChart::toGeoPosition( const QPointF& _rqPointFDrawPosition ) const
{
  return( poChartGDAL->toGeoPosition( poChartGDAL->getDatPosition() + ( _rqPointFDrawPosition - qRectFDrawArea.center() ) / poChartGDAL->getZoom() ) );
}

QPointF CChart::toDrawPosition( const CDataPosition& _roGeoPosition ) const
{
  return( ( poChartGDAL->toDatPosition( _roGeoPosition ) - poChartGDAL->getDatPosition() ) * poChartGDAL->getZoom() + qRectFDrawArea.center() );
}

void CChart::move( const QPointF& _rqPointFDrawPositionOffset )
{
  qPointFDatPosition += _rqPointFDrawPositionOffset / poChartGDAL->getZoom();
}

void CChart::draw()
{
  // Constant drawing resources
  static const QPointF __qPointFCrosshairA1(5,0), __qPointFCrosshairA2(15,0);
  static const QPointF __qPointFCrosshairB1(0,5), __qPointFCrosshairB2(0,15);
  static const QPointF __qPointFCrosshairC1(-5,0), __qPointFCrosshairC2(-15,0);
  static const QPointF __qPointFCrosshairD1(0,-5), __qPointFCrosshairD2(0,-15);

  // Draw
  QPainter __qPainter;
  __qPainter.begin( this );
  __qPainter.setRenderHints( QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing );
  // ... chart
  poChartGDAL->draw( &__qPainter, qPointFDatPosition, fdZoom );
  // ... overlays
  QVCTRuntime::useLandmarkOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useRouteOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useTrackOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useVesselOverlay()->draw( this, &__qPainter );
  QVCTRuntime::usePointerOverlay()->draw( this, &__qPainter );
  // ... center crosshair
  QPen __qPen;
  __qPen.setColor( QColor( 0, 0, 0, 128 ) );
  __qPen.setWidth( 1 );
  __qPainter.setPen( __qPen );
  QPointF __qPointF = qRectFDrawArea.center();
  __qPainter.drawPoint( __qPointF );
  __qPainter.setPen( __qPen );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairA1, __qPointF + __qPointFCrosshairA2 );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairB1, __qPointF + __qPointFCrosshairB2 );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairC1, __qPointF + __qPointFCrosshairC2 );
  __qPainter.drawLine( __qPointF + __qPointFCrosshairD1, __qPointF + __qPointFCrosshairD2 );
  // ... [end]
  __qPainter.end();
}

void CChart::print( QPrinter* _pqPrinter )
{
  // Printer
  QPainter __qPainter;
  __qPainter.begin( _pqPrinter );
  __qPainter.setRenderHints( QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing );
  // ... chart
  poChartGDAL->draw( &__qPainter, qPointFDatPosition, fdZoom );
  // ... overlays
  QVCTRuntime::useLandmarkOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useRouteOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useTrackOverlay()->draw( this, &__qPainter );
  QVCTRuntime::useVesselOverlay()->draw( this, &__qPainter );
  QVCTRuntime::usePointerOverlay()->draw( this, &__qPainter );
  // ... [end]
  __qPainter.end();
}
