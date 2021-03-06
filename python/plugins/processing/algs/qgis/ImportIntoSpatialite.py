# -*- coding: utf-8 -*-

"""
***************************************************************************
    ImportIntoSpatialite.py
    ---------------------
    Date                 : October 2016
    Copyright            : (C) 2016 by Mathieu Pellerin
    Email                : nirvn dot asia at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Mathieu Pellerin'
__date__ = 'October 2016'
__copyright__ = '(C) 2012, Mathieu Pellerin'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

from qgis.core import (QgsDataSourceUri,
                       QgsVectorLayerImport,
                       QgsApplication,
                       QgsProcessingUtils)

from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.GeoAlgorithmExecutionException import GeoAlgorithmExecutionException
from processing.core.parameters import ParameterBoolean
from processing.core.parameters import ParameterVector
from processing.core.parameters import ParameterString
from processing.core.parameters import ParameterTableField
from processing.tools import spatialite


class ImportIntoSpatialite(GeoAlgorithm):

    DATABASE = 'DATABASE'
    TABLENAME = 'TABLENAME'
    INPUT = 'INPUT'
    OVERWRITE = 'OVERWRITE'
    CREATEINDEX = 'CREATEINDEX'
    GEOMETRY_COLUMN = 'GEOMETRY_COLUMN'
    LOWERCASE_NAMES = 'LOWERCASE_NAMES'
    DROP_STRING_LENGTH = 'DROP_STRING_LENGTH'
    FORCE_SINGLEPART = 'FORCE_SINGLEPART'
    PRIMARY_KEY = 'PRIMARY_KEY'
    ENCODING = 'ENCODING'

    def icon(self):
        return QgsApplication.getThemeIcon("/providerQgis.svg")

    def svgIconPath(self):
        return QgsApplication.iconPath("providerQgis.svg")

    def group(self):
        return self.tr('Database')

    def name(self):
        return 'importintospatialite'

    def displayName(self):
        return self.tr('Import into Spatialite')

    def defineCharacteristics(self):
        self.addParameter(ParameterVector(self.INPUT, self.tr('Layer to import')))
        self.addParameter(ParameterVector(self.DATABASE, self.tr('File database'), False, False))
        self.addParameter(ParameterString(self.TABLENAME, self.tr('Table to import to (leave blank to use layer name)'), optional=True))
        self.addParameter(ParameterTableField(self.PRIMARY_KEY, self.tr('Primary key field'), self.INPUT, optional=True))
        self.addParameter(ParameterString(self.GEOMETRY_COLUMN, self.tr('Geometry column'), 'geom'))
        self.addParameter(ParameterString(self.ENCODING, self.tr('Encoding'), 'UTF-8', optional=True))
        self.addParameter(ParameterBoolean(self.OVERWRITE, self.tr('Overwrite'), True))
        self.addParameter(ParameterBoolean(self.CREATEINDEX, self.tr('Create spatial index'), True))
        self.addParameter(ParameterBoolean(self.LOWERCASE_NAMES, self.tr('Convert field names to lowercase'), True))
        self.addParameter(ParameterBoolean(self.DROP_STRING_LENGTH, self.tr('Drop length constraints on character fields'), False))
        self.addParameter(ParameterBoolean(self.FORCE_SINGLEPART, self.tr('Create single-part geometries instead of multi-part'), False))

    def processAlgorithm(self, context, feedback):
        database = self.getParameterValue(self.DATABASE)
        uri = QgsDataSourceUri(database)
        if uri.database() is '':
            if '|layerid' in database:
                database = database[:database.find('|layerid')]
            uri = QgsDataSourceUri('dbname=\'%s\'' % (database))
        db = spatialite.GeoDB(uri)

        overwrite = self.getParameterValue(self.OVERWRITE)
        createIndex = self.getParameterValue(self.CREATEINDEX)
        convertLowerCase = self.getParameterValue(self.LOWERCASE_NAMES)
        dropStringLength = self.getParameterValue(self.DROP_STRING_LENGTH)
        forceSinglePart = self.getParameterValue(self.FORCE_SINGLEPART)
        primaryKeyField = self.getParameterValue(self.PRIMARY_KEY) or 'id'
        encoding = self.getParameterValue(self.ENCODING)

        layerUri = self.getParameterValue(self.INPUT)
        layer = QgsProcessingUtils.mapLayerFromString(layerUri, context)

        table = self.getParameterValue(self.TABLENAME)
        if table:
            table.strip()
        if not table or table == '':
            table = layer.name()
        table = table.replace(' ', '').lower()
        providerName = 'spatialite'

        geomColumn = self.getParameterValue(self.GEOMETRY_COLUMN)
        if not geomColumn:
            geomColumn = 'the_geom'

        options = {}
        if overwrite:
            options['overwrite'] = True
        if convertLowerCase:
            options['lowercaseFieldNames'] = True
            geomColumn = geomColumn.lower()
        if dropStringLength:
            options['dropStringConstraints'] = True
        if forceSinglePart:
            options['forceSinglePartGeometryType'] = True

        # Clear geometry column for non-geometry tables
        if not layer.hasGeometryType():
            geomColumn = None

        uri = db.uri
        uri.setDataSource('', table, geomColumn, '', primaryKeyField)

        if encoding:
            layer.setProviderEncoding(encoding)

        (ret, errMsg) = QgsVectorLayerImport.importLayer(
            layer,
            uri.uri(),
            providerName,
            self.crs,
            False,
            False,
            options,
        )
        if ret != 0:
            raise GeoAlgorithmExecutionException(
                self.tr('Error importing to Spatialite\n{0}').format(errMsg))

        if geomColumn and createIndex:
            db.create_spatial_index(table, geomColumn)
