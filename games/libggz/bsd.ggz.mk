# $FreeBSD: games/libggz/bsd.ggz.mk 340722 2014-01-22 17:00:46Z mat $

# common settings

GGZ_VERSION=	0.0.14.1

MASTER_SITES=	\
	http://mirrors.dotsrc.org/ggzgamingzone/ggz/%SUBDIR%/ \
	http://ftp.ggzgamingzone.org/pub/ggz/%SUBDIR%/
MASTER_SITE_SUBDIR=	${PORTVERSION}
DIST_SUBDIR=	ggz

# components section

_USE_GGZ_ALL=	core client

core_LIB_DEPENDS=	ggz.5:${PORTSDIR}/games/libggz

client_LIB_DEPENDS=	ggzcore.9:${PORTSDIR}/games/ggz-client-libs

.for component in ${USE_GGZ}
LIB_DEPENDS+=	${${component}_LIB_DEPENDS}
. endfor

.for component in ${USE_GGZ}
. if ${_USE_GGZ_ALL:M${component}}==""
IGNORE=	cannot install: Unknown component ${component}
. endif
.endfor
