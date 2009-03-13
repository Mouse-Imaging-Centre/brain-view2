#!/bin/bash

# create the directory for frameworks
fdir=brain-view2.app/Contents/Frameworks

if [ ! -d $fdir ]
    then
    mkdir $fdir
fi

# the icon
cp bv2icon.icns brain-view2.app/Contents/Resources

# copy the MINC libraries
mdir=/usr/local/minc2/lib

cp ${mdir}/libnetcdf.4.dylib $fdir
cp ${mdir}/libhdf5.0.dylib $fdir
cp ${mdir}/libminc2.1.dylib $fdir
cp ${mdir}/libvolume_io2.1.dylib $fdir

# change where the libraries are read from
bv=brain-view2.app/Contents/MacOS/brain-view2

install_name_tool -change ${mdir}/libvolume_io2.1.dylib \
    @executable_path/../Frameworks/libvolume_io2.1.dylib $bv
install_name_tool -change ${mdir}/libminc2.1.dylib \
    @executable_path/../Frameworks/libminc2.1.dylib $bv
install_name_tool -change ${mdir}/libhdf5.0.dylib \
    @executable_path/../Frameworks/libhdf5.0.dylib $bv
install_name_tool -change ${mdir}/libnetcdf.4.dylib \
    @executable_path/../Frameworks/libnetcdf.4.dylib $bv

install_name_tool -change ${mdir}/libhdf5.0.dylib \
    @executable_path/../Frameworks/libhdf5.0.dylib \
    ${fdir}/libvolume_io2.1.dylib
install_name_tool -change ${mdir}/libnetcdf.4.dylib \
    @executable_path/../Frameworks/libnetcdf.4.dylib \
    ${fdir}/libvolume_io2.1.dylib

install_name_tool -change ${mdir}/libhdf5.0.dylib \
    @executable_path/../Frameworks/libhdf5.0.dylib \
    ${fdir}/libminc2.1.dylib
install_name_tool -change ${mdir}/libnetcdf.4.dylib \
    @executable_path/../Frameworks/libnetcdf.4.dylib \
    ${fdir}/libminc2.1.dylib

# PCRE
cp /usr/local//lib/libpcre++.0.dylib $fdir
cp /usr/local//lib/libpcre.0.dylib $fdir

install_name_tool -change /usr/local//lib/libpcre++.0.dylib \
    @executable_path/../Frameworks/libpcre++.0.dylib $bv
install_name_tool -change /usr/local//lib/libpcre.0.dylib \
    @executable_path/../Frameworks/libpcre.0.dylib $bv

install_name_tool -change /usr/local//lib/libpcre.0.dylib \
    @executable_path/../Frameworks/libpcre.0.dylib \
    brain-view2.app/Contents/Frameworks/libpcre++.0.dylib

# Coin
ditto /Library/Frameworks/Inventor.framework/ $fdir/Inventor.framework/
ditto /Library/Frameworks/Quarter.framework/ $fdir/Quarter.framework/

install_name_tool -change Inventor.framework/Versions/C/Inventor \
    @executable_path/../Frameworks/Inventor.framework/Versions/C/Inventor $bv
install_name_tool -change Quarter.framework/Versions/A/Quarter \
    @executable_path/../Frameworks/Quarter.framework/Versions/A/Quarter $bv

# Qt
ditto /Library/Frameworks/QtCore.framework/ $fdir/QtCore.framework
ditto /Library/Frameworks/QtGui.framework/ $fdir/QtGui.framework
ditto /Library/Frameworks/QtOpenGL.framework/ $fdir/QtOpenGL.framework
ditto /Library/Frameworks/QtXml.framework/ $fdir/QtXml.framework
ditto /Library/Frameworks/QtNetwork.framework/ $fdir/QtNetwork.framework
ditto /Library/Frameworks/QtSql.framework/ $fdir/QtSql.framework
ditto /Library/Frameworks/QtScript.framework/ $fdir/QtScript.framework
ditto /Library/Frameworks/QtDesigner.framework $fdir/QtDesigner.framework

install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $bv
install_name_tool -change QtGui.framework/Versions/4/QtGui \
    @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $bv
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL \
    @executable_path/../Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL $bv

# make changes to links inside the Quarter framework
quart=brain-view2.app/Contents/Frameworks/Quarter.framework/Versions/A/Quarter

install_name_tool -change Inventor.framework/Versions/C/Inventor \
    @executable_path/../Frameworks/Inventor.framework/Versions/C/Inventor $quart
install_name_tool -change QtGui.framework/Versions/4/QtGui \
    @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $quart
install_name_tool -change QtOpenGL.framework/Versions/4/QtOpenGL \
    @executable_path/../Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL $quart
install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $quart
install_name_tool -change QtXml.framework/Versions/4/QtXml \
    @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml $quart
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork \
    @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork \
    $quart
install_name_tool -change QtSql.framework/Versions/4/QtSql \
    @executable_path/../Frameworks/QtSql.framework/Versions/4/QtSql $quart
install_name_tool -change QtScript.framework/Versions/4/QtScript \
    @executable_path/../Frameworks/QtScript.framework/Versions/4/QtScript $quart
install_name_tool -change QtDesigner.framework/Versions/4/QtDesigner \
    @executable_path/../Frameworks/QtDesigner.framework/Versions/4/QtDesigner \
    $quart

# make changes inside Qt libraries
install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtGui.framework/Versions/4/QtGui

install_name_tool -change QtGui.framework/Versions/4/QtGui \
    @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
    brain-view2.app/Contents/Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL
install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtOpenGL.framework/Versions/4/QtOpenGL

install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtXml.framework/Versions/4/QtXml

install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtNetwork.framework/Versions/4/QtNetwork

install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtSql.framework/Versions/4/QtSql

install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtScript.framework/Versions/4/QtScript

install_name_tool -change QtCore.framework/Versions/4/QtCore \
    @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore \
    brain-view2.app/Contents/Frameworks/QtDesigner.framework/Versions/4/QtDesigner
install_name_tool -change QtGui.framework/Versions/4/QtGui \
    @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui \
    brain-view2.app/Contents/Frameworks/QtDesigner.framework/Versions/4/QtDesigner
install_name_tool -change QtScript.framework/Versions/4/QtScript \
    @executable_path/../Frameworks/QtScript.framework/Versions/4/QtScript \
    brain-view2.app/Contents/Frameworks/QtDesigner.framework/Versions/4/QtDesigner
install_name_tool -change QtXml.framework/Versions/4/QtXml \
    @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml \
    brain-view2.app/Contents/Frameworks/QtDesigner.framework/Versions/4/QtDesigner
