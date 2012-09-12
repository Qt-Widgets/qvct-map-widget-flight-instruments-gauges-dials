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

// QT
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>

// QVCT
#include "QVCTRuntime.hpp"
#include "overlays/vessel/CVesselPoint.hpp"
#include "overlays/vessel/CVesselPointEditView.hpp"


//------------------------------------------------------------------------------
// CONSTRUCTORS / DESTRUCTOR
//------------------------------------------------------------------------------

CVesselPointEditView::CVesselPointEditView( CVesselPoint* _poVesselPoint )
  : COverlayObjectEditView( _poVesselPoint )
{
  constructLayout();
}

void CVesselPointEditView::constructLayout()
{
  // Create layout
  QVBoxLayout* __pqVBoxLayout = new QVBoxLayout( this );

  // Add header
  QFont __qFontHeader;
  __qFontHeader.setPixelSize( 16 );
  __qFontHeader.setBold( true );
  QHBoxLayout* __pqHBoxLayoutHeader = new QHBoxLayout();
  QLabel* __pqLabelIcon = new QLabel( this );
  __pqLabelIcon->setPixmap( QPixmap( ":icons/32x32/vessel_point.png" ) );
  __pqHBoxLayoutHeader->addWidget( __pqLabelIcon, 0, Qt::AlignTop );
  QLabel* __pqLabelEdit = new QLabel( tr("Edit")+"...", this );
  __pqLabelEdit->setFont( __qFontHeader );
  __pqHBoxLayoutHeader->addWidget( __pqLabelEdit, 1 );
  // ... [end]
  __pqVBoxLayout->addLayout( __pqHBoxLayoutHeader );

  // Add data
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;
  QFormLayout* __pqFormLayout = new QFormLayout();

  // ... name
  pqLineEditName = new QLineEdit( this );
  pqLineEditName->setToolTip( tr("Name") );
  pqLineEditName->setText( __poVesselPoint->getName() );
  __pqFormLayout->addRow( tr("Name")+":", pqLineEditName );
  bool __bPositionDefined = __poVesselPoint->CDataPosition::operator!=( CDataPosition::UNDEFINED );

  // ... position
  QHBoxLayout* __pqHBoxLayoutPosition = new QHBoxLayout();
  pqLineEditLongitude = new QLineEdit( this );
  pqLineEditLongitude->setToolTip( tr("Longitude") );
  if( __bPositionDefined ) pqLineEditLongitude->setText( CUnitPosition::toString( __poVesselPoint->getLongitude(), CUnitPosition::LONGITUDE ) );
  __pqHBoxLayoutPosition->addWidget( pqLineEditLongitude );
  pqLineEditLatitude = new QLineEdit( this );
  pqLineEditLatitude->setToolTip( tr("Latitude") );
  if( __bPositionDefined ) pqLineEditLatitude->setText( CUnitPosition::toString( __poVesselPoint->getLatitude(), CUnitPosition::LATITUDE ) );
  __pqHBoxLayoutPosition->addWidget( pqLineEditLatitude );
  pqLineEditElevation = new QLineEdit( this );
  pqLineEditElevation->setToolTip( tr("Elevation") );
  if( __poVesselPoint->getElevation() != CDataPosition::UNDEFINED_ELEVATION ) pqLineEditElevation->setText( CUnitElevation::toString( __poVesselPoint->getElevation() ) );
  __pqHBoxLayoutPosition->addWidget( pqLineEditElevation );
  __pqFormLayout->addRow( tr("Position")+":", __pqHBoxLayoutPosition );

  // ... date/time
  QHBoxLayout* __pqHBoxLayoutDate = new QHBoxLayout();
  pqLineEditDate = new QLineEdit( this );
  pqLineEditDate->setToolTip( tr("Date") );
  if( __poVesselPoint->getTime() != CDataTime::UNDEFINED_TIME ) pqLineEditDate->setText( CUnitDate::toString( __poVesselPoint->getTime() ) );
  __pqHBoxLayoutDate->addWidget( pqLineEditDate );
  pqLineEditTime = new QLineEdit( this );
  pqLineEditTime->setToolTip( tr("Time") );
  if( __poVesselPoint->getTime() != CDataTime::UNDEFINED_TIME ) pqLineEditTime->setText( CUnitTime::toString( __poVesselPoint->getTime() ) );
  __pqHBoxLayoutDate->addWidget( pqLineEditTime );
  __pqFormLayout->addRow( tr("Date & Time")+":", __pqHBoxLayoutDate );

  // ... ground course
  QHBoxLayout* __pqHBoxLayoutGroundCourse = new QHBoxLayout();
  pqLineEditGroundBearing = new QLineEdit( this );
  pqLineEditGroundBearing->setToolTip( tr("Bearing") );
  if( __poVesselPoint->GroundCourse.getBearing() != CDataCourse::UNDEFINED_BEARING ) pqLineEditGroundBearing->setText( CUnitBearing::toString( __poVesselPoint->GroundCourse.getBearing() ) );
  __pqHBoxLayoutGroundCourse->addWidget( pqLineEditGroundBearing );
  pqLineEditGroundSpeed = new QLineEdit( this );
  pqLineEditGroundSpeed->setToolTip( tr("Speed (horizontal)") );
  if( __poVesselPoint->GroundCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED ) pqLineEditGroundSpeed->setText( CUnitSpeed::toString( __poVesselPoint->GroundCourse.getSpeed() ) );
  __pqHBoxLayoutGroundCourse->addWidget( pqLineEditGroundSpeed );
  pqLineEditGroundSpeedVertical = new QLineEdit( this );
  pqLineEditGroundSpeedVertical->setToolTip( tr("Speed (vertical)") );
  if( __poVesselPoint->GroundCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED ) pqLineEditGroundSpeedVertical->setText( CUnitSpeedVertical::toString( __poVesselPoint->GroundCourse.getSpeedVertical() ) );
  __pqHBoxLayoutGroundCourse->addWidget( pqLineEditGroundSpeedVertical );
  __pqFormLayout->addRow( tr("Ground Course")+":", __pqHBoxLayoutGroundCourse );

  // ... apparent course
  QHBoxLayout* __pqHBoxLayoutApparentCourse = new QHBoxLayout();
  pqLineEditApparentBearing = new QLineEdit( this );
  pqLineEditApparentBearing->setToolTip( tr("Bearing") );
  if( __poVesselPoint->ApparentCourse.getBearing() != CDataCourse::UNDEFINED_BEARING ) pqLineEditApparentBearing->setText( CUnitBearing::toString( __poVesselPoint->ApparentCourse.getBearing() ) );
  __pqHBoxLayoutApparentCourse->addWidget( pqLineEditApparentBearing );
  pqLineEditApparentSpeed = new QLineEdit( this );
  pqLineEditApparentSpeed->setToolTip( tr("Speed (horizontal)") );
  if( __poVesselPoint->ApparentCourse.getSpeed() != CDataCourse::UNDEFINED_SPEED ) pqLineEditApparentSpeed->setText( CUnitSpeed::toString( __poVesselPoint->ApparentCourse.getSpeed() ) );
  __pqHBoxLayoutApparentCourse->addWidget( pqLineEditApparentSpeed );
  pqLineEditApparentSpeedVertical = new QLineEdit( this );
  pqLineEditApparentSpeedVertical->setToolTip( tr("Speed (vertical)") );
  if( __poVesselPoint->ApparentCourse.getSpeedVertical() != CDataCourse::UNDEFINED_SPEED ) pqLineEditApparentSpeedVertical->setText( CUnitSpeedVertical::toString( __poVesselPoint->ApparentCourse.getSpeedVertical() ) );
  __pqHBoxLayoutApparentCourse->addWidget( pqLineEditApparentSpeedVertical );
  __pqFormLayout->addRow( tr("Apparent Course")+":", __pqHBoxLayoutApparentCourse );

  // ... type/symbol
  QHBoxLayout* __pqHBoxLayoutTypeSymbol = new QHBoxLayout();
  pqLineEditType = new QLineEdit( this );
  pqLineEditType->setToolTip( tr("Type") );
  pqLineEditType->setText( __poVesselPoint->getType() );
  __pqHBoxLayoutTypeSymbol->addWidget( pqLineEditType );
  pqLineEditSymbol = new QLineEdit( this );
  pqLineEditSymbol->setToolTip( tr("Symbol") );
  pqLineEditSymbol->setText( __poVesselPoint->getSymbol() );
  __pqHBoxLayoutTypeSymbol->addWidget( pqLineEditSymbol );
  __pqFormLayout->addRow( tr("Type & Symbol")+":", __pqHBoxLayoutTypeSymbol );

  // ... description
  pqTextEditDescription = new QTextEdit( this );
  pqTextEditDescription->setToolTip( tr("Comment") );
  pqTextEditDescription->setAcceptRichText( false );
  pqTextEditDescription->setPlainText( __poVesselPoint->getDescription() );
  __pqFormLayout->addRow( tr("Description")+":", pqTextEditDescription );

  // ... comment
  pqTextEditComment = new QTextEdit( this );
  pqTextEditComment->setToolTip( tr("Comment") );
  pqTextEditComment->setAcceptRichText( false );
  pqTextEditComment->setPlainText( __poVesselPoint->getComment() );
  __pqFormLayout->addRow( tr("Comment")+":", pqTextEditComment );

  // ... url
  pqLineEditUrl = new QLineEdit( this );
  pqLineEditUrl->setToolTip( tr("URL") );
  pqLineEditUrl->setText( __poVesselPoint->getUrl() );
  __pqFormLayout->addRow( tr("URL")+":", pqLineEditUrl );

  // ... track recording
  pqSpinBoxTrackRecordRate = new QSpinBox( this );
  pqSpinBoxTrackRecordRate->setRange( 1, 300 );
  pqSpinBoxTrackRecordRate->setToolTip( tr("Track Recording Rate [seconds]") );
  pqSpinBoxTrackRecordRate->setValue( __poVesselPoint->getTrackRecordRate() );
  __pqFormLayout->addRow( tr("Track Recording Rate")+":", pqSpinBoxTrackRecordRate );

  // ... [end]
  __pqVBoxLayout->addLayout( __pqFormLayout );

  // Add buttons
  QDialogButtonBox* __pqDialogButtonBox = new QDialogButtonBox( QDialogButtonBox::Cancel|QDialogButtonBox::Save, Qt::Horizontal, this );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(accepted()), this, SLOT(accept()) );
  QDialog::connect( __pqDialogButtonBox, SIGNAL(rejected()), this, SLOT(reject()) );
  __pqVBoxLayout->addWidget( __pqDialogButtonBox );

  // Set the layout
  COverlayObjectEditView::setLayout( __pqVBoxLayout );

}


//------------------------------------------------------------------------------
// METHODS: QDialog (override)
//------------------------------------------------------------------------------

void CVesselPointEditView::accept()
{
  CVesselPoint* __poVesselPoint = (CVesselPoint*)poOverlayObject;

  // Check data
  bool __bOK;

  // ... position
  double __fdLongitude = CDataPosition::UNDEFINED_LONGITUDE;
  double __fdLatitude = CDataPosition::UNDEFINED_LATITUDE;
  double __fdElevation = CDataPosition::UNDEFINED_ELEVATION;
  if( !pqLineEditLongitude->text().isEmpty() && !pqLineEditLatitude->text().isEmpty() )
  {
    __fdLongitude = CUnitPosition::fromString( pqLineEditLongitude->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditLongitude->text() );
      return;
    }
    __fdLatitude = CUnitPosition::fromString( pqLineEditLatitude->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditLatitude->text() );
      return;
    }
  }
  if( !pqLineEditElevation->text().isEmpty() )
  {
    __fdElevation = CUnitElevation::fromString( pqLineEditElevation->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditElevation->text() );
      return;
    }
  }

  // ... time
  double __fdTime = CDataTime::UNDEFINED_TIME;
  double __fdDate = CDataTime::UNDEFINED_TIME;
  if( !pqLineEditDate->text().isEmpty() )
  {
    __fdDate = CUnitDate::fromString( pqLineEditDate->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditDate->text() );
      return;
    }
  }
  if( __fdDate != CDataTime::UNDEFINED_TIME && !pqLineEditTime->text().isEmpty() )
  {
    __fdTime = CUnitTime::fromString( pqLineEditTime->text(), __fdDate, &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditTime->text() );
      return;
    }
  }

  // ... course
  double __fdGroundBearing = CDataCourse::UNDEFINED_BEARING;
  double __fdGroundSpeed = CDataCourse::UNDEFINED_SPEED;
  double __fdGroundSpeedVertical = CDataCourse::UNDEFINED_SPEED;
  double __fdApparentBearing = CDataCourse::UNDEFINED_BEARING;
  double __fdApparentSpeed = CDataCourse::UNDEFINED_SPEED;
  double __fdApparentSpeedVertical = CDataCourse::UNDEFINED_SPEED;
  if( !pqLineEditGroundBearing->text().isEmpty() )
  {
    __fdGroundBearing = CUnitBearing::fromString( pqLineEditGroundBearing->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditGroundBearing->text() );
      return;
    }
  }
  if( !pqLineEditGroundSpeed->text().isEmpty() )
  {
    __fdGroundSpeed = CUnitSpeed::fromString( pqLineEditGroundSpeed->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditGroundSpeed->text() );
      return;
    }
  }
  if( !pqLineEditGroundSpeedVertical->text().isEmpty() )
  {
    __fdGroundSpeedVertical = CUnitSpeedVertical::fromString( pqLineEditGroundSpeedVertical->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditGroundSpeedVertical->text() );
      return;
    }
  }
  if( !pqLineEditApparentBearing->text().isEmpty() )
  {
    __fdApparentBearing = CUnitBearing::fromString( pqLineEditApparentBearing->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditApparentBearing->text() );
      return;
    }
  }
  if( !pqLineEditApparentSpeed->text().isEmpty() )
  {
    __fdApparentSpeed = CUnitSpeed::fromString( pqLineEditApparentSpeed->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditApparentSpeed->text() );
      return;
    }
  }
  if( !pqLineEditApparentSpeedVertical->text().isEmpty() )
  {
    __fdApparentSpeedVertical = CUnitSpeedVertical::fromString( pqLineEditApparentSpeedVertical->text(), &__bOK );
    if( !__bOK )
    {
      QVCTRuntime::useMainWindow()->parseError( pqLineEditApparentSpeedVertical->text() );
      return;
    }
  }

  // Set data
  // ... name
  __poVesselPoint->setText( CVesselOverlay::NAME, pqLineEditName->text() ); // NOTE: Item's name will be updated via QTreeWidget::itemChanged()
  // ... position
  __poVesselPoint->setPosition( __fdLongitude, __fdLatitude, __fdElevation );
  // ... time
  __poVesselPoint->setTime( __fdTime );
  // ... course
  __poVesselPoint->GroundCourse.setCourse( __fdGroundBearing, __fdGroundSpeed, __fdGroundSpeedVertical );
  __poVesselPoint->ApparentCourse.setCourse( __fdApparentBearing, __fdApparentSpeed, __fdApparentSpeedVertical );
  // ... type
  __poVesselPoint->setType( pqLineEditType->text() );
  // ... description
  __poVesselPoint->setDescription( pqTextEditDescription->toPlainText() );
  // ... comment
  __poVesselPoint->setComment( pqTextEditComment->toPlainText() );
  // ... symbol
  __poVesselPoint->setSymbol( pqLineEditSymbol->text() );
  // ... url
  __poVesselPoint->setUrl( pqLineEditUrl->text() );
  // ... track recording
  __poVesselPoint->setTrackRecordRate( pqSpinBoxTrackRecordRate->value() );

  // Done
  QDialog::accept();
}
