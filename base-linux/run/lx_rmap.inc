#
# \brief  Test for Linux-specific region map
# \author Christian Helmuth
# \date   2013-09-06
#

assert_spec linux

#
# Build
#

set build_components { core init }

lappend_if [expr {$test_type eq "static"}]  build_components test/lx_rmap/static
lappend_if [expr {$test_type eq "dynamic"}] build_components test/lx_rmap/dynamic

build $build_components

create_boot_directory

#
# Config
#

set config {
<config>
	<parent-provides>
		<service name="ROM"/>
		<service name="RM"/>
		<service name="CAP"/>
		<service name="LOG"/>
		<service name="IO_PORT"/>
		<service name="IO_MEM"/>
		<service name="IRQ"/>
		<service name="CPU"/>
		<service name="SIGNAL"/>
	</parent-provides>
	<default-route>
		<any-service> <parent/> <any-child/> </any-service>
	</default-route>}

append_if [expr {$test_type eq "static"}] config {
	<start name="test-lx_rmap_static">}
append_if [expr {$test_type eq "dynamic"}] config {
	<start name="test-lx_rmap_dynamic">}

append config {
		<resource name="RAM" quantum="8M"/>
	</start>
</config>}

install_config $config

#
# Boot modules
#

set boot_modules { core init}

lappend_if [expr {$test_type eq "static"}]  boot_modules test-lx_rmap_static
lappend_if [expr {$test_type eq "dynamic"}] boot_modules test-lx_rmap_dynamic
lappend_if [expr {$test_type eq "dynamic"}] boot_modules ld.lib.so
lappend_if [expr {$test_type eq "dynamic"}] boot_modules libc.lib.so
lappend_if [expr {$test_type eq "dynamic"}] boot_modules libc_log.lib.so
lappend_if [expr {$test_type eq "dynamic"}] boot_modules libm.lib.so

build_boot_image $boot_modules

#
# Execute test
#

run_genode_until forever

# vi: set ft=tcl :
