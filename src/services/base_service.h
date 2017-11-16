#ifndef SSF_SERVICES_BASE_SERVICE_H_
#define SSF_SERVICES_BASE_SERVICE_H_

#include <cstdint>

#include <map>
#include <string>
#include <memory>

#include <boost/asio/io_service.hpp>
#include <boost/system/error_code.hpp>

#include "common/boost/fiber/stream_fiber.hpp"
#include "common/boost/fiber/datagram_fiber.hpp"

namespace ssf {

//----------------------------------------------------------------------------
/// Base class for services
template <typename Demux>
class BaseService : public std::enable_shared_from_this<BaseService<Demux>> {
 public:
  using BaseServicePtr = std::shared_ptr<BaseService<Demux>>;

  using socket_type = typename Demux::socket_type;
  using fiber = typename boost::asio::fiber::stream_fiber<socket_type>::socket;
  using fiber_datagram =
      typename boost::asio::fiber::datagram_fiber<socket_type>::socket;
  using fiber_acceptor =
      typename boost::asio::fiber::stream_fiber<socket_type>::acceptor;
  using endpoint =
      typename boost::asio::fiber::stream_fiber<socket_type>::endpoint;
  using datagram_endpoint =
      typename boost::asio::fiber::datagram_fiber<socket_type>::endpoint;

  using Parameters = std::map<std::string, std::string>;

 public:
  virtual ~BaseService() {}

  virtual void start(boost::system::error_code&) = 0;
  virtual void stop(boost::system::error_code&) = 0;
  virtual uint32_t service_type_id() = 0;


  void set_local_id(uint32_t local_id) { local_id_ = local_id; }

  uint32_t local_id() { return local_id_; }

 protected:
  /// Constructor
  /**
  * @param io_service the io_service used for all asynchronous operations
  * @param demux the demultiplexer used to send and receive data
  */
  BaseService(boost::asio::io_service& io_service, Demux& demux)
      : io_service_(io_service), demux_(demux) {}

  /// Accessor for the io_service
  boost::asio::io_service& get_io_service() { return io_service_; }

  /// Accessor for the demultiplexer
  Demux& get_demux() { return demux_; }

 private:
  BaseService(const BaseService&) = delete;
  BaseService& operator=(const BaseService&) = delete;

 private:
  boost::asio::io_service& io_service_;
  Demux& demux_;
  uint32_t local_id_;
};

}  // ssf

#endif  // SSF_SERVICES_BASE_SERVICE_H_
