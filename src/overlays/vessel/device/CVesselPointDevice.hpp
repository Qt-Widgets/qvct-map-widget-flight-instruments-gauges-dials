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

#ifndef QVCT_CVESSELPOINTDEVICE_HPP
#define QVCT_CVESSELPOINTDEVICE_HPP

// QT
#include <QDataStream>
#include <QDomElement> // QtXml module
#include <QPainter>
#include <QTreeWidgetItem>
#include <QXmlStreamWriter>

// QVCT
#include "devices/data/CDeviceDataFix.hpp"
#include "overlays/COverlayItem.hpp"
class CChart;
class CDevice;
class CDeviceDataSkyView;


/// [UI] Vessel device
/**
 *  This class implements the link between a vessel and a device (source), used
 *  to update the vessel's geographical position and (ground/apparent) course.
 *  @author Cedric Dufour <http://cedric.dufour.name>
 */
class CVesselPointDevice: public COverlayItem, public CDeviceDataFix
{
  Q_OBJECT

  //------------------------------------------------------------------------------
  // FIELDS
  //------------------------------------------------------------------------------

private:
  /// Dynamic flag (vessel device automatically generated by a dynamic flotilla)
  bool bDynamic;

  /// Corresponding device's pointer
  CDevice* poDevice;
  /// [Flag] Use this device/source to synchronize the vessel's position
  bool bSynchronizePosition;
  /// [Flag] Use this device/source to synchronize the vessel's elevation
  bool bSynchronizeElevation;
  /// [Flag] Use this device/source to synchronize the vessel's time
  bool bSynchronizeTime;
  /// [Flag] Use this device/source to synchronize the vessel's ground bearing
  bool bSynchronizeGroundBearing;
  /// [Flag] Use this device/source to synchronize the vessel's ground horizontal speed
  bool bSynchronizeGroundSpeed;
  /// [Flag] Use this device/source to synchronize the vessel's ground vertical speed
  bool bSynchronizeGroundSpeedVertical;
  /// [Flag] Use this device/source to synchronize the vessel's apparent bearing
  bool bSynchronizeApparentBearing;
  /// [Flag] Use this device/source to synchronize the vessel's apparent horizontal speed
  bool bSynchronizeApparentSpeed;
  /// [Flag] Use this device/source to synchronize the vessel's apparent vertical speed
  bool bSynchronizeApparentSpeedVertical;
  /// [Flag] Use this device/source to synchronize the vessel's additional textual data string
  bool bSynchronizeText;


  //------------------------------------------------------------------------------
  // CONSTRUCTORS / DESTRUCTOR
  //------------------------------------------------------------------------------

public:
  CVesselPointDevice( const QString& _rqsName, const QString& _rqsSourceName, bool _bDynamic = false );
  virtual ~CVesselPointDevice() {};


  //------------------------------------------------------------------------------
  // METHODS: COverlayObject (implement/override)
  //------------------------------------------------------------------------------

public:
  virtual void serialize( QDataStream& _rqDataStream ) const {};
  virtual void unserialize( QDataStream& _rqDataStream ) {};

public:
  virtual COverlay* useOverlay() { return (COverlay*)QTreeWidgetItem::parent()->parent()->parent(); };

public:
  virtual void draw( const CChart* _poChart, QPainter* _pqPainter ) {};
  virtual void showDetail();
  virtual void showEdit();


  //------------------------------------------------------------------------------
  // METHODS
  //------------------------------------------------------------------------------

  // SIGNALS
signals:
  /// Signal emitted when views should refresh this object's corresponding content
  void signalRefreshContent();

  // SLOTS
private slots:
  /// Slot to handle object destruction
  void slotDestroyed( QObject* _pqObject );

private slots:
  /// Slot to handle fix data (sent by device)
  void slotDataFix( const CDeviceDataFix& _roDeviceDataFix );
  /// Slot to handle sky view data (sent by device)
  void slotDataSkyView( const CDeviceDataSkyView& _roDeviceDataSkyView );

  // SETTERS
public:
  /// Sets the synchronizazion flags
  void setSynchronized( bool _bSynchronizePosition, bool _bSynchronizeElevation, bool _bSynchronizeTime,
                        bool _bSynchronizeGroundBearing, bool _bSynchronizeGroundSpeed, bool _bSynchronizeGroundSpeedVertical,
                        bool _bSynchronizeApparentBearing, bool _bSynchronizeApparentSpeed, bool _bSynchronizeApparentSpeedVertical,
                        bool _bSynchronizeText );

  // GETTERS
public:
  /// Returns whether the device is dynamically generated
  bool isDynamic() { return bDynamic; };
  /// Returns whether the actual device is connected
  bool isDeviceConnected() const { return poDevice; };

  /// Returns whether this device/source is used to synchronize the vessel's position
  bool isSynchronizedPosition() { return bSynchronizePosition; };
  /// Returns whether this device/source is used to synchronize the vessel's elevation
  bool isSynchronizedElevation() { return bSynchronizeElevation; };
  /// Returns whether this device/source is used to synchronize the vessel's time
  bool isSynchronizedTime() { return bSynchronizeTime; };
  /// Returns whether this device/source is used to synchronize the vessel's ground bearing
  bool isSynchronizedGroundBearing() { return bSynchronizeGroundBearing; };
  /// Returns whether this device/source is used to synchronize the vessel's ground horizontal speed
  bool isSynchronizedGroundSpeed() { return bSynchronizeGroundSpeed; };
  /// Returns whether this device/source is used to synchronize the vessel's ground vertical speed
  bool isSynchronizedGroundSpeedVertical() { return bSynchronizeGroundSpeedVertical; };
  /// Returns whether this device/source is used to synchronize the vessel's apparent bearing
  bool isSynchronizedApparentBearing() { return bSynchronizeApparentBearing; };
  /// Returns whether this device/source is used to synchronize the vessel's apparent horizontal speed
  bool isSynchronizedApparentSpeed() { return bSynchronizeApparentSpeed; };
  /// Returns whether this device/source is used to synchronize the vessel's apparent vertical speed
  bool isSynchronizedApparentSpeedVertical() { return bSynchronizeApparentSpeedVertical; };
  /// Returns whether this device/source is used to synchronize the vessel's additional textual data string
  bool isSynchronizedText() { return bSynchronizeText; };

  // OTHER
public:
  /// Connects to the actual device
  bool connectDevice();
  /// Dicconnects from the actual device
  void disconnectDevice();

public:
  /// Retrieves this object's content from the given QVCT source (file)
  void parseQVCT( const QDomElement& _rqDomElement );
  /// Stores this object's content to the given QVCT destination (file)
  void dumpQVCT( QXmlStreamWriter & _rqXmlStreamWriter ) const;

};

#endif // QVCT_CVESSELPOINTDEVICE_HPP
