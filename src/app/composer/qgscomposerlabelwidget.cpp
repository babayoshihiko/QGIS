/***************************************************************************
                         qgscomposerlabelwidget.cpp
                         --------------------------
    begin                : June 10, 2008
    copyright            : (C) 2008 by Marco Hugentobler
    email                : marco dot hugentobler at karto dot baug dot ethz dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgscomposerlabelwidget.h"
#include "qgscomposerlabel.h"
#include "qgscomposeritemwidget.h"
#include "qgscomposition.h"
#include "qgsexpressionbuilderdialog.h"
#include "qgisgui.h"

#include <QColorDialog>
#include <QFontDialog>
#include <QWidget>

QgsComposerLabelWidget::QgsComposerLabelWidget( QgsComposerLabel *label ): QgsComposerItemBaseWidget( nullptr, label ), mComposerLabel( label )
{
  setupUi( this );
  setPanelTitle( tr( "Label properties" ) );

  //add widget for general composer item properties
  QgsComposerItemWidget *itemPropertiesWidget = new QgsComposerItemWidget( this, label );
  mainLayout->addWidget( itemPropertiesWidget );

  mFontColorButton->setColorDialogTitle( tr( "Select font color" ) );
  mFontColorButton->setContext( QStringLiteral( "composer" ) );

  mMarginXDoubleSpinBox->setClearValue( 0.0 );
  mMarginYDoubleSpinBox->setClearValue( 0.0 );

  if ( mComposerLabel )
  {
    setGuiElementValues();
    connect( mComposerLabel, &QgsComposerObject::itemChanged, this, &QgsComposerLabelWidget::setGuiElementValues );
  }
}

void QgsComposerLabelWidget::on_mHtmlCheckBox_stateChanged( int state )
{
  if ( mComposerLabel )
  {
    mVerticalAlignementLabel->setDisabled( state );
    mTopRadioButton->setDisabled( state );
    mMiddleRadioButton->setDisabled( state );
    mBottomRadioButton->setDisabled( state );

    mComposerLabel->beginCommand( tr( "Label text HTML state changed" ), QgsComposerMergeCommand::ComposerLabelSetText );
    mComposerLabel->blockSignals( true );
    mComposerLabel->setHtmlState( state );
    mComposerLabel->setText( mTextEdit->toPlainText() );
    mComposerLabel->update();
    mComposerLabel->blockSignals( false );
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mTextEdit_textChanged()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label text changed" ), QgsComposerMergeCommand::ComposerLabelSetText );
    mComposerLabel->blockSignals( true );
    mComposerLabel->setText( mTextEdit->toPlainText() );
    mComposerLabel->update();
    mComposerLabel->blockSignals( false );
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mFontButton_clicked()
{
  if ( mComposerLabel )
  {
    bool ok;
    QFont newFont = QgisGui::getFont( ok, mComposerLabel->font() );
    if ( ok )
    {
      mComposerLabel->beginCommand( tr( "Label font changed" ) );
      mComposerLabel->setFont( newFont );
      mComposerLabel->update();
      mComposerLabel->endCommand();
    }
  }
}

void QgsComposerLabelWidget::on_mMarginXDoubleSpinBox_valueChanged( double d )
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label margin changed" ) );
    mComposerLabel->setMarginX( d );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mMarginYDoubleSpinBox_valueChanged( double d )
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label margin changed" ) );
    mComposerLabel->setMarginY( d );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mFontColorButton_colorChanged( const QColor &newLabelColor )
{
  if ( !mComposerLabel )
  {
    return;
  }

  mComposerLabel->beginCommand( tr( "Label color changed" ), QgsComposerMergeCommand::ComposerLabelFontColor );
  mComposerLabel->setFontColor( newLabelColor );
  mComposerLabel->update();
  mComposerLabel->endCommand();
}

void QgsComposerLabelWidget::on_mInsertExpressionButton_clicked()
{
  if ( !mComposerLabel )
  {
    return;
  }

  QString selText = mTextEdit->textCursor().selectedText();

  // edit the selected expression if there's one
  if ( selText.startsWith( QLatin1String( "[%" ) ) && selText.endsWith( QLatin1String( "%]" ) ) )
    selText = selText.mid( 2, selText.size() - 4 );

  // use the atlas coverage layer, if any
  QgsVectorLayer *coverageLayer = atlasCoverageLayer();
  QgsExpressionContext context = mComposerLabel->createExpressionContext();
  QgsExpressionBuilderDialog exprDlg( coverageLayer, selText, this, QStringLiteral( "generic" ), context );

  exprDlg.setWindowTitle( tr( "Insert expression" ) );
  if ( exprDlg.exec() == QDialog::Accepted )
  {
    QString expression =  exprDlg.expressionText();
    if ( !expression.isEmpty() )
    {
      mComposerLabel->beginCommand( tr( "Insert expression" ) );
      mTextEdit->insertPlainText( "[%" + expression + "%]" );
      mComposerLabel->endCommand();
    }
  }
}

void QgsComposerLabelWidget::on_mCenterRadioButton_clicked()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
    mComposerLabel->setHAlign( Qt::AlignHCenter );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mRightRadioButton_clicked()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
    mComposerLabel->setHAlign( Qt::AlignRight );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mLeftRadioButton_clicked()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
    mComposerLabel->setHAlign( Qt::AlignLeft );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mTopRadioButton_clicked()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
    mComposerLabel->setVAlign( Qt::AlignTop );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mBottomRadioButton_clicked()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
    mComposerLabel->setVAlign( Qt::AlignBottom );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::on_mMiddleRadioButton_clicked()
{
  if ( mComposerLabel )
  {
    mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
    mComposerLabel->setVAlign( Qt::AlignVCenter );
    mComposerLabel->update();
    mComposerLabel->endCommand();
  }
}

void QgsComposerLabelWidget::setGuiElementValues()
{
  blockAllSignals( true );
  mTextEdit->setPlainText( mComposerLabel->text() );
  mTextEdit->moveCursor( QTextCursor::End, QTextCursor::MoveAnchor );
  mMarginXDoubleSpinBox->setValue( mComposerLabel->marginX() );
  mMarginYDoubleSpinBox->setValue( mComposerLabel->marginY() );
  mHtmlCheckBox->setChecked( mComposerLabel->htmlState() );
  mTopRadioButton->setChecked( mComposerLabel->vAlign() == Qt::AlignTop );
  mMiddleRadioButton->setChecked( mComposerLabel->vAlign() == Qt::AlignVCenter );
  mBottomRadioButton->setChecked( mComposerLabel->vAlign() == Qt::AlignBottom );
  mLeftRadioButton->setChecked( mComposerLabel->hAlign() == Qt::AlignLeft );
  mCenterRadioButton->setChecked( mComposerLabel->hAlign() == Qt::AlignHCenter );
  mRightRadioButton->setChecked( mComposerLabel->hAlign() == Qt::AlignRight );
  mFontColorButton->setColor( mComposerLabel->fontColor() );


  mVerticalAlignementLabel->setDisabled( mComposerLabel->htmlState() );
  mTopRadioButton->setDisabled( mComposerLabel->htmlState() );
  mMiddleRadioButton->setDisabled( mComposerLabel->htmlState() );
  mBottomRadioButton->setDisabled( mComposerLabel->htmlState() );

  blockAllSignals( false );
}

void QgsComposerLabelWidget::blockAllSignals( bool block )
{
  mTextEdit->blockSignals( block );
  mHtmlCheckBox->blockSignals( block );
  mMarginXDoubleSpinBox->blockSignals( block );
  mMarginYDoubleSpinBox->blockSignals( block );
  mTopRadioButton->blockSignals( block );
  mMiddleRadioButton->blockSignals( block );
  mBottomRadioButton->blockSignals( block );
  mLeftRadioButton->blockSignals( block );
  mCenterRadioButton->blockSignals( block );
  mRightRadioButton->blockSignals( block );
  mFontColorButton->blockSignals( block );
}
