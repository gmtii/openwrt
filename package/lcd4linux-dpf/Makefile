#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lcd4linux
PKG_REV:=1196
PKG_RELEASE:=3
PKG_VERSION:=r$(PKG_REV)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=https://ssl.bulix.org/svn/lcd4linux/trunk/

PKG_SOURCE_SUBDIR:=lcd4linux-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)

PKG_SOURCE_PROTO:=svn

LCD4LINUX_DRIVERS:= \
	DPF \
	PNG

LCD4LINUX_PLUGINS:= \
	apm \
	asterisk \
	button_exec \
	cpuinfo \
	dbus \
	diskstats \
	dvb \
	event \
	exec \
	fifo \
	file \
	gps \
	hddtemp \
	huawei \
	i2c_sensors \
	iconv \
	imon \
	isdn \
	kvv \
	loadavg \
	netdev \
	netinfo \
	meminfo \
	mpd \
	mpris_dbus \
	mysql \
	netdev \
	pop3 \
	ppp \
	proc_stat \
	qnaplog \
	seti \
	statfs \
	uname \
	uptime \
	w1retap \
	$(if $(CONFIG_BROKEN),wireless) \
	xmms \
#	python \

PKG_FIXUP:=libtool
PKG_INSTALL:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DEPENDS:= \
	libgd \
	libmpdclient \
	libmpd \
	libncurses \
	libusb-compat


PKG_CONFIG_DEPENDS:= \
	$(patsubst %,CONFIG_LCD4LINUX_CUSTOM_DRIVER_%,$(LCD4LINUX_DRIVERS)) \
	$(patsubst %,CONFIG_LCD4LINUX_CUSTOM_PLUGIN_%,$(LCD4LINUX_PLUGINS)) \


include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/lcd4linux/Default
  SECTION:=utils
  CATEGORY:=Utilities
  MAINTAINER:=irimi@openwrt.org
  TITLE:=LCD display utility
  URL:=http://lcd4linux.bulix.org/
endef


define Build/Prepare
	echo "Prepare"
	mkdir -p $(PKG_BUILD_DIR)/dpflib
	$(CP) ./files/dpflib_03devel/* $(PKG_BUILD_DIR)/dpflib

	$(call Build/Prepare/Default)
endef


define Build/Compile
	$(MAKE)  CC=$(TARGET_CC) \
		LD=$(TARGET_CC) \
		FPIC="$(FPIC)" \
		CFLAGS="$(FPIC) $(TARGET_CFLAGS) -I$(STAGING_DIR)/usr/include  -D_GNU_SOURCE" \
		LDFLAGS="$(TARGET_LDFLAGS) -L." \
		-C $(PKG_BUILD_DIR)/dpflib

	#this is required for optional pydpflib
	$(CP) ./files/dpflib_03devel/dpf.h $(STAGING_DIR)/usr/include
	$(CP) ./files/dpflib_03devel/usbuser.h $(STAGING_DIR)/usr/include
	$(CP) ./files/dpflib_03devel/spiflash.h $(STAGING_DIR)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dpflib/libdpf.a $(STAGING_DIR)/usr/lib/libdpf.a

#	$(call Build/Compile/Default)
endef

define Package/lcd4linux/Default/description
 LCD4Linux is a small program that grabs information from the kernel and 
#TARGET_CFLAGS += $(FPIC)
 some subsystems and displays it on an external liquid crystal display.
endef


define Package/lcd4linux-dpf
$(call Package/lcd4linux/Default)
  DEPENDS:= \
	+LCD4LINUX_CUSTOM_NEEDS_libgd:libgd \
	$(if $(ICONV_FULL),+LCD4LINUX_CUSTOM_NEEDS_libiconv:libiconv-full) \
	+LCD4LINUX_CUSTOM_NEEDS_libmpdclient:libmpdclient \
	+LCD4LINUX_CUSTOM_NEEDS_libncurses:libncurses \
	+LCD4LINUX_CUSTOM_NEEDS_libusb-compat:libusb-compat \
	+LCD4LINUX_CUSTOM_NEEDS_libmpd:libmpd
#	+LCD4LINUX_CUSTOM_NEEDS_dpflib:dpflib
  MENU:=1
  PROVIDES:=lcd4linux
  VARIANT=dpf
endef

define Package/lcd4linux-dpf/config
	source "$(SOURCE)/Config.in"
endef

define Package/lcd4linux-dpf/description
$(call Package/lcd4linux/Default/description)
 
 This package contains a customized version of LCD4Linux for LCDs which requires 
 the DPF Appotech AX206 driver.
endef

CONFIGURE_ARGS+= \
	--disable-rpath \

EXTRA_LDFLAGS+= -Wl,-rpath-link,$(STAGING_DIR)/usr/lib
EXTRA_CFLAGS+="-liconv"

ifeq ($(BUILD_VARIANT),dpf)

  LCD4LINUX_CUSTOM_PLUGINS:= $(strip $(foreach c, $(LCD4LINUX_PLUGINS), \
    $(if $(CONFIG_LCD4LINUX_CUSTOM_PLUGIN_$(c)),$(c)) \
  ))
  ifeq ($(LCD4LINUX_CUSTOM_PLUGINS),)
    LCD4LINUX_CUSTOM_PLUGINS:=sample
  endif

EXTRA_CPPFLAGS+= -I$(STAGING_DIR)/usr/include
EXTRA_LDFLAGS+= -L$$(STAGING_DIR)/usr/lib
EXTRA_CPPFLAGS+= -I$(PKG_BUILD_DIR)/dpflib
EXTRA_LDFLAGS+= -L$(PKG_BUILD_DIR)/dpflib

  CONFIGURE_ARGS+= \
	--with-drivers="DPF" \
	--with-plugins="$(LCD4LINUX_CUSTOM_PLUGINS)" \


  ifneq ($(CONFIG_LCD4LINUX_CUSTOM_NEEDS_libiconv),)
    CONFIGURE_ARGS+= --with-libiconv-prefix="$(ICONV_PREFIX)"
  else
    CONFIGURE_ARGS+= --without-libiconv-prefix
  endif

    CONFIGURE_ARGS+= --without-python
    CONFIGURE_ARGS+= --without-x
    #CONFIGURE_ARGS+="LIBS=-ldpf -lusb -lgd"

endif


define Package/lcd4linux/conffiles
/etc/lcd4linux.conf
endef

define Package/lcd4linux/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/lcd4linux $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) ./files/lcd4linux.conf $(1)/etc/lcd4linux.conf
		chmod 600 $(1)/etc/lcd4linux.conf
#	$(INSTALL_CONF) $(PKG_BUILD_DIR)/lcd4linux.conf.sample $(1)/etc/lcd4linux.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/lcd4linux.init $(1)/etc/init.d/lcd4linux
	$(SED) "s|^\(Display 'GLCD2USB'\)|#\1|g" \
	    -e "s|^\(Layout 'TestLayer'\)|#\1|g" \
	    -e "s|^#\(Display 'Image'\)|\1|g" \
	    -e "s|^#\(Layout 'Default'\)|\1|g" \
	     $(1)/etc/lcd4linux.conf
endef


define Package/lcd4linux/dpflib

endef

Package/lcd4linux-dpf/dpflib = $(Package/lcd4linux/dpflib)
Package/lcd4linux-dpf/conffiles = $(Package/lcd4linux/conffiles)
Package/lcd4linux-dpf/install = $(Package/lcd4linux/install)


$(eval $(call BuildPackage,lcd4linux-dpf))
