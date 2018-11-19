#include <eosiolib/asset.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/singleton.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/transaction.h>

using namespace eosio;
using namespace std;

CONTRACT bugeos : public eosio::contract
{
  public:
    using contract::contract;
    bugeos(name receiver, name code, datastream<const char *> ds)
        : contract(receiver, code, ds)
        , _tbets(receiver, receiver.value){};

    
    void transfer(const name &from, const name &to, const asset &quantity,
                  const string &memo);

  private:
    TABLE tbet
    {
        uint64_t key;
        name player1;
        name player2;
        uint8_t status; // 1 waiting, 2 matched
        uint64_t timestamp;

        uint64_t primary_key() const { return key; }
        EOSLIB_SERIALIZE(tbet, (key)(player1)(player2)(status)(timestamp))
    };

    
    typedef eosio::multi_index<name("tbet"), tbet> tbetting;
    tbetting _tbets;

    bool find_waiting_bet(uint64_t &key)
    {
        for (auto &item : _tbets)
        {
            if (item.status == 1)
            {
                key = item.key;
                return true;
            }
        }
        return false;
    }
};
