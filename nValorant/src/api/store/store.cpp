#include "store.h"
#include "offers.h"
#include "wallet.h"
#include "player/player.h"


#include <iostream>

COffers::COffers(CApi* pApi) 
{

}

CStoreV2::CStoreV2(CApi* pApi) 
{

}

CWallet::CWallet(CApi* pApi) 
{
	if (!pApi->isSuccessful)
		return;

	CSession session(pApi);
	cpr::Response wallet =
		cpr::Get(cpr::Url
			{
				fmt::format("https://pd.{0}.a.pvp.net/store/v1/wallet/{1}", pApi->playerRegion, session.puuid)
			},
			cpr::Header
			{
				{
					"X-Riot-Entitlements-JWT", pApi->entitlementToken
				}
			},
			cpr::Bearer
			{
				pApi->riotToken
			});

	nlohmann::json walletParsed = nlohmann::json::parse(wallet.text);
	points = walletParsed["Balances"]["85ad13f7-3d1b-5128-9eb2-7cd8ee0b5741"].get<std::int32_t>();
	radianite = walletParsed["Balances"]["e59aa87c-4cbf-517a-5983-6e81511be9b7"].get<std::int32_t>();
}