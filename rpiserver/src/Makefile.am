##
## @info   rpiserver ver.1.0
## @author Vladimir Roncevic <elektron.ronca@gmail.com>
## @brief  Process this file with automake to produce Makefile.in
##

AM_CPPFLAGS = \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	$(NLOHMANN_JSON_CFLAGS)

AM_CXXFLAGS = \
	-Wall \
	-Wextra \
	-pedantic \
	-std=c++17 \
	$(NLOHMANN_JSON_CFLAGS)

bin_PROGRAMS = rpiserver

rpiserver_SOURCES = \
	rpi_server_config.h \
	logger/rpi_logger.h \
	logger/rpi_logger.cc \
	config/rpi_server_options.h \
	config/rpi_server_options.cc \
	core/rpi_channel_manager.h \
	core/rpi_channel_manager.cc \
	protocol/rpi_json_rpc.h \
	protocol/rpi_json_rpc.cc \
	server/rpi_client_session.h \
	server/rpi_client_session.cc \
	server/rpi_tcp_server.h \
	server/rpi_tcp_server.cc \
	main.cc

rpiserver_LDADD = \
	$(PTHREAD_LIBS) \
	$(NLOHMANN_JSON_LIBS)

install-exec-hook:
	@echo "Installing rpiserver binary to $(bindir)"
