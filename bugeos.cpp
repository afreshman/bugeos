#include "bugeos.hpp"

void bugeos::transfer(const name &from, const name &to, const asset &quantity, const string &memo)
{
    require_auth(from);
    if ("bet" == memo)
    {
        uint64_t key;
        bool exist = find_waiting_bet(key);
        if (exist)
        {
            auto itr = _tbets.find(key);
            _tbets.modify(itr, get_self(), [&](auto &p){
                p.timestamp = current_time();
                p.status = 2; // matched
                p.player2 = from;
            });
        }
        else
        {
            _tbets.emplace(get_self(), [&](auto &p){
                p.key = _tbets.available_primary_key();
                p.timestamp = current_time();
                p.status = 1; // waiting for match
                p.player1 = from;
            });
        }
    }
}

extern "C"
{
    void apply(uint64_t receiver, uint64_t code, uint64_t action)
    {
        if (code == "eosio.token"_n.value && action == "transfer"_n.value)
        {
            execute_action(name(receiver), name(code), &bugeos::transfer);
        }
        eosio_exit(0);
    }
};
