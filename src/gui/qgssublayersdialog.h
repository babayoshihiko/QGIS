/***************************************************************************
    qgssublayersdialog.h  - dialog for selecting sublayers
    ---------------------
    begin                : January 2009
    copyright            : (C) 2009 by Florian El Ahdab
    email                : felahdab at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSUBLAYERSDIALOG_H
#define QGSSUBLAYERSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <ui_qgssublayersdialogbase.h>
#include "qgscontexthelp.h"
#include "qgis_gui.h"

/** \ingroup gui
 * \class QgsSublayersDialog
 */
class GUI_EXPORT QgsSublayersDialog : public QDialog, private Ui::QgsSublayersDialogBase
{
    Q_OBJECT
  public:

    enum ProviderType
    {
      Ogr,
      Gdal,
      Vsifile
    };

    //! A structure that defines layers for the purpose of this dialog
    //! \since QGIS 2.16
    typedef struct LayerDefinition
    {
      LayerDefinition() : layerId( -1 ), count( -1 ) {}

      int layerId;        //!< Identifier of the layer (one unique layer id may have multiple types though)
      QString layerName;  //!< Name of the layer (not necessarily unique)
      int count;          //!< Number of features (might be unused)
      QString type;       //!< Extra type depending on the use (e.g. geometry type for vector sublayers)
    } LayerDefinition;

    //! List of layer definitions for the purpose of this dialog
    //! \since QGIS 2.16
    typedef QList<LayerDefinition> LayerDefinitionList;

    QgsSublayersDialog( ProviderType providerType, const QString &name, QWidget *parent = nullptr, Qt::WindowFlags fl = 0 );
    ~QgsSublayersDialog();

    //! Populate the table with layers
    //! \since QGIS 2.16
    void populateLayerTable( const LayerDefinitionList &list );

    //! Returns list of selected layers
    //! \since QGIS 2.16
    LayerDefinitionList selection();

    //! Set if we should display the add to group checkbox
    //! \since QGIS 3.0
    void setShowAddToGroupCheckbox( bool showAddToGroupCheckbox ) { mShowAddToGroupCheckbox = showAddToGroupCheckbox; }

    //! If we should display the add to group checkbox
    //! \since QGIS 3.0
    bool showAddToGroupCheckbox() const { return mShowAddToGroupCheckbox; }

    //! If we should add layers in a group
    //! \since QGIS 3.0
    bool addToGroupCheckbox() const { return mCheckboxAddToGroup->isChecked(); }

  public slots:
    void on_buttonBox_helpRequested() { QgsContextHelp::run( metaObject()->className() ); }
    int exec();

  protected:
    QString mName;
    QStringList mSelectedSubLayers;
    bool mShowCount = false;  //!< Whether to show number of features in the table
    bool mShowType = false;   //!< Whether to show type in the table

  private:

    bool mShowAddToGroupCheckbox = false;   //!< Whether to show the add to group checkbox
    QCheckBox *mCheckboxAddToGroup = nullptr;
};

#endif
