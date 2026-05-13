#include <xrpld/core/Config.h>
#include <xrpld/peerfinder/PeerfinderManager.h>

namespace xrpl::PeerFinder {

Config
makeConfig(
    xrpl::Config const& cfg,
    std::uint16_t port,
    bool validationPublicKey,
    int ipLimit,
    bool verifyEndpoints)
{
    PeerLimitConfig limits;
    if ((cfg.PEERS_OUT_MAX == 0u) && (cfg.PEERS_IN_MAX == 0u))
    {
        limits.maxPeers = cfg.PEERS_MAX;
    }
    else
    {
        limits.inPeers = cfg.PEERS_IN_MAX;
        limits.outPeers = cfg.PEERS_OUT_MAX;
    }

    return Config::makeConfig(
        cfg.PEER_PRIVATE,
        cfg.standalone(),
        limits,
        port,
        validationPublicKey,
        ipLimit,
        verifyEndpoints);
}

}  // namespace xrpl::PeerFinder
