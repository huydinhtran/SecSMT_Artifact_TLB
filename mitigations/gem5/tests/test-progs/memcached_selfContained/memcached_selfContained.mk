# MEMCACHED_SELFCONTAINED_VERSION:=20.11.3
MEMCACHED_SELFCONTAINED_SITE:="$(BR2_EXTERNAL_DPDK_GEM5_PATH)/package/memcached_selfContained/memcached-selfContained"
MEMCACHED_SELFCONTAINED_SITE_METHOD:=local
MEMCACHED_SELFCONTAINED_INSTALL_TARGET:=YES
#MEMCACHED_SELFCONTAINED_DEPENDENCIES:= libpcap

define MEMCACHED_SELFCONTAINED_BUILD_CMDS
	cd $(@D) && ./configure
	$(MAKE) -C $(@D)
endef

define MEMCACHED_SELFCONTAINED_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/memcached $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/dataset_keys $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/dataset_vals $(TARGET_DIR)/usr/bin
endef	



$(eval $(generic-package))