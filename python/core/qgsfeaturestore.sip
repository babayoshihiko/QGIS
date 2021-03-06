/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/qgsfeaturestore.h                                           *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/


class QgsFeatureStore : QgsFeatureSink
{
%Docstring
 A container for features with the same fields and crs.
%End

%TypeHeaderCode
#include "qgsfeaturestore.h"
%End
  public:
    QgsFeatureStore();
%Docstring
Constructor
%End

    QgsFeatureStore( const QgsFields &fields, const QgsCoordinateReferenceSystem &crs );
%Docstring
Constructor
%End

    QgsFields fields() const;
%Docstring
 Returns the store's field list.
.. seealso:: setFields()
 :rtype: QgsFields
%End

    void setFields( const QgsFields &fields );
%Docstring
 Sets the store's ``fields``. Every contained feature's fields will be reset to match ``fields``.
.. seealso:: fields()
%End

    QgsCoordinateReferenceSystem crs() const;
%Docstring
 Returns the store's coordinate reference system.
.. seealso:: setCrs()
 :rtype: QgsCoordinateReferenceSystem
%End

    void setCrs( const QgsCoordinateReferenceSystem &crs );
%Docstring
 Sets the store's ``crs``.
.. seealso:: crs()
%End

    virtual bool addFeature( QgsFeature &feature );

    virtual bool addFeatures( QgsFeatureList &features );


    QgsFeatureList features() const;
%Docstring
 Returns the list of features contained in the store.
 :rtype: QgsFeatureList
%End

    void setParams( const QMap<QString, QVariant> &parameters );
%Docstring
 Sets a map of optional ``parameters`` for the store.
.. seealso:: params()
%End

    QMap<QString, QVariant> params() const;
%Docstring
 Returns the map of optional parameters.
.. seealso:: setParams()
 :rtype: QMap<str, QVariant>
%End

};

typedef QList<QgsFeatureStore> QgsFeatureStoreList;



/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/qgsfeaturestore.h                                           *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/
