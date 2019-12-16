#########################################################################################
##
##  VLIBS codebase, NIIAS
##
##  GNU Lesser General Public License Usage
##  This file may be used under the terms of the GNU Lesser General Public License
##  version 3 as published by the Free Software Foundation and appearing in the file
##  LICENSE.LGPL3 included in the packaging of this file. Please review the following
##  information to ensure the GNU Lesser General Public License version 3 requirements
##  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#########################################################################################
# vpoll.pri

#========================================================================================
isEmpty(qi_vpoll) {
    qi_vpoll = 1;
    isEmpty(qi_not_print_pri_messages): message("=== vpoll appended ===")

    isEmpty(VLIBS_DIR): error("vpoll: Need VLIBS_DIR correct path.")

    include( $$VLIBS_DIR/qmake/vposix.pri )

    INCLUDEPATH += $$VLIBS_DIR/vpoll

    HEADERS     += $$VLIBS_DIR/vpoll/vinvoke_iface.h
    SOURCES     += $$VLIBS_DIR/vpoll/vinvoke_iface.cpp

    HEADERS     += $$VLIBS_DIR/vpoll/vapplication.h
    SOURCES     += $$VLIBS_DIR/vpoll/vapplication.cpp

    HEADERS     += $$VLIBS_DIR/vpoll/vthread.h
    SOURCES     += $$VLIBS_DIR/vpoll/vthread.cpp

    HEADERS     += $$VLIBS_DIR/vpoll/impl_vpoll/poll_context.h
    SOURCES     += $$VLIBS_DIR/vpoll/impl_vpoll/poll_context.cpp

    OTHER_FILES += $$VLIBS_DIR/vpoll/vpoll.cmake
    OTHER_FILES += $$VLIBS_DIR/vpoll/README
}
# vpoll.pri
#========================================================================================
