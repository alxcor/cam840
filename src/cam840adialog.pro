TEMPLATE	 = lib
CONFIG		+= qt warn_on thread plugin ${BUILDMODE}
PROJECT		 = cam840a
TARGET		 = $(BUILDMODE)/output/appl/cam840a
LANGUAGE	 = C++
 
 
UI_DIR 		 = .ui
MOC_DIR		 = .moc
OBJECTS_DIR	 = .obj
 
 
INCLUDEPATH	+= /user/oapack/gui/include  \
		   	   /user/oapack/include   \
		   	   /user/oapack/linux/ace/embedded/${BUILDMODE}
 
 
#für Qt/embedded
unix:LIBS   += -L/user/oapack/bin/${BUILDMODE}
unix:LIBS   += -L${QTDIR}/lib \
               -l${LIBQT_CORE} \
               -l${LIBQT_GUI} \
               -l${LIBQT_XML}

LIBS        += -lslgfw -lslgfwwidget -lslcap -lsltrc -lslmd -lsltrp -lslhmiutilitieslib -lslaesvcadapter -lslfsfilesvcadapter -lsltraceadapter -lslarchiveadapter

SOURCES	+=	cam840aplugin.cpp \
			slexversion.cpp \
			chartxy.cpp \
			cam840aform.cpp

HEADERS	+=	chartxy.h \
		cam840aform.h

FORMS	+=	
