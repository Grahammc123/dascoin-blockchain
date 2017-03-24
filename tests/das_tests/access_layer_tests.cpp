/**
 * DASCOIN!
 */

#include <boost/test/unit_test.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/access_layer.hpp>
#include <graphene/chain/exceptions.hpp>

#include <graphene/chain/queue_objects.hpp>

#include "../common/database_fixture.hpp"

using namespace graphene::chain;
using namespace graphene::chain::test;

BOOST_FIXTURE_TEST_SUITE( dascoin_tests, database_fixture )

BOOST_FIXTURE_TEST_SUITE( access_layer_unit_tests, database_fixture )

BOOST_AUTO_TEST_CASE( get_free_cycle_balance_non_existant_id_unit_test )
{ try {
  auto bogus_id = account_id_type(999999);

  auto res = _dal.get_free_cycle_balance(bogus_id);
  BOOST_CHECK( res.account_id == bogus_id );
  BOOST_CHECK( !res.result.valid() );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_free_cycle_balance_unit_test )
{ try {
  VAULT_ACTOR(vault)

  auto res = _dal.get_free_cycle_balance(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  BOOST_CHECK_EQUAL( res.result->value, 0 );

  adjust_cycles(vault_id, 100);

  res = _dal.get_free_cycle_balance(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  BOOST_CHECK_EQUAL( res.result->value, 100 );

  adjust_cycles(vault_id, 300);

  res = _dal.get_free_cycle_balance(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  BOOST_CHECK_EQUAL( res.result->value, 400 );

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), vault_id, 200, 200, ""));

  res = _dal.get_free_cycle_balance(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  BOOST_CHECK_EQUAL( res.result->value, 400 );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_all_cycle_balances_non_existant_id_unit_test )
{ try {
  auto bogus_id = account_id_type(999999);

  auto res = _dal.get_all_cycle_balances(bogus_id);
  BOOST_CHECK( res.account_id == bogus_id );
  BOOST_CHECK( !res.result.valid() );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_all_cycle_balances_unit_test )
{ try {
  VAULT_ACTOR(vault)

  adjust_cycles(vault_id, 100);

  auto res = _dal.get_all_cycle_balances(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  auto cycle_vec = *res.result;
  BOOST_CHECK_EQUAL( cycle_vec.size(), 1 );
  BOOST_CHECK_EQUAL( cycle_vec[0].cycles.value, 100 );
  BOOST_CHECK_EQUAL( cycle_vec[0].frequency_lock.value, 0 );

  adjust_cycles(vault_id, 1000);
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), vault_id, 200, 200, ""));
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), vault_id, 210, 200, ""));
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), vault_id, 220, 200, ""));

  res = _dal.get_all_cycle_balances(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  cycle_vec = *res.result;
  BOOST_CHECK_EQUAL( cycle_vec.size(), 4 );
  BOOST_CHECK_EQUAL( cycle_vec[0].cycles.value, 1100 );
  BOOST_CHECK_EQUAL( cycle_vec[0].frequency_lock.value, 0 );
  BOOST_CHECK_EQUAL( cycle_vec[1].cycles.value, 200 );
  BOOST_CHECK_EQUAL( cycle_vec[1].frequency_lock.value, 200 );
  BOOST_CHECK_EQUAL( cycle_vec[2].cycles.value, 210 );
  BOOST_CHECK_EQUAL( cycle_vec[2].frequency_lock.value, 200 );
  BOOST_CHECK_EQUAL( cycle_vec[3].cycles.value, 220 );
  BOOST_CHECK_EQUAL( cycle_vec[3].frequency_lock.value, 200 );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_dascoin_balance_non_existant_id_unit_test )
{ try {
  auto bogus_id = account_id_type(999999);

  auto res = _dal.get_dascoin_balance(bogus_id);
  BOOST_CHECK( res.account_id == bogus_id );
  BOOST_CHECK( !res.result.valid() );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_dascoin_balances )
{ try {
  VAULT_ACTOR(vault)

  db.issue_asset(vault_id, 1000 * DASCOIN_DEFAULT_ASSET_PRECISION, get_dascoin_asset_id(), 0);

  auto res = _dal.get_dascoin_balance(vault_id);
  BOOST_CHECK( res.account_id == vault_id );
  BOOST_CHECK_EQUAL( res.result->value, 1000 * DASCOIN_DEFAULT_ASSET_PRECISION );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_free_cycle_balances_for_accounts_unit_test )
{ try {
  VAULT_ACTORS((first)(second)(third))
  auto bogus_id = account_id_type(99999999);

  adjust_cycles(second_id, 200);
  adjust_cycles(third_id, 300);

  auto res_vec = _dal.get_free_cycle_balances_for_accounts({first_id, second_id, third_id, bogus_id});

  BOOST_CHECK_EQUAL( res_vec.size(), 4 );

  BOOST_CHECK( res_vec[0].account_id == first_id );
  BOOST_CHECK_EQUAL( res_vec[0].result->value, 0 );

  BOOST_CHECK( res_vec[1].account_id == second_id );
  BOOST_CHECK_EQUAL( res_vec[1].result->value, 200 );

  BOOST_CHECK( res_vec[2].account_id == third_id );
  BOOST_CHECK_EQUAL( res_vec[2].result->value, 300 );

  BOOST_CHECK( res_vec[3].account_id == bogus_id );
  BOOST_CHECK( !res_vec[3].result.valid() );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_all_cycle_balances_for_accounts_unit_test )
{ try {
  VAULT_ACTORS((zero)(somefree)(somequeue)(both))
  auto bogus_id = account_id_type(99999);

  adjust_cycles( somefree_id, 100 );
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), somequeue_id, 200, 200, ""));
  adjust_cycles( both_id, 300 );
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), both_id, 310, 200, ""));

  auto result_vec = _dal.get_all_cycle_balances_for_accounts({zero_id, somefree_id, somequeue_id, both_id, bogus_id});

  BOOST_CHECK_EQUAL( result_vec.size(), 5 );

  // Zero has balance of 0 with lock 0:
  BOOST_CHECK( result_vec[0].account_id == zero_id );
  auto agreement_vec = *result_vec[0].result;
  BOOST_CHECK_EQUAL( agreement_vec.size(), 1 );
  BOOST_CHECK_EQUAL( agreement_vec[0].cycles.value, 0 );
  BOOST_CHECK_EQUAL( agreement_vec[0].frequency_lock.value, 0 );

  // Somefree has balance of 100 with lock 0:
  BOOST_CHECK( result_vec[1].account_id == somefree_id );
  agreement_vec = *result_vec[1].result;
  BOOST_CHECK_EQUAL( agreement_vec.size(), 1 );
  BOOST_CHECK_EQUAL( agreement_vec[0].cycles.value, 100 );
  BOOST_CHECK_EQUAL( agreement_vec[0].frequency_lock.value, 0 );

  // Somequeue has balance of 0 with frequency lock 0 and balance 200 with lock 200:
  BOOST_CHECK( result_vec[2].account_id == somequeue_id );
  agreement_vec = *result_vec[2].result;
  BOOST_CHECK_EQUAL( agreement_vec.size(), 2 );
  BOOST_CHECK_EQUAL( agreement_vec[0].cycles.value, 0 );
  BOOST_CHECK_EQUAL( agreement_vec[0].frequency_lock.value, 0 );
  BOOST_CHECK_EQUAL( agreement_vec[1].cycles.value, 200 );
  BOOST_CHECK_EQUAL( agreement_vec[1].frequency_lock.value, 200 );

  // Both has balance of 300 with lock 0 and balance of 310 with lock 200:
  BOOST_CHECK( result_vec[3].account_id == both_id );
  agreement_vec = *result_vec[3].result;
  BOOST_CHECK_EQUAL( agreement_vec.size(), 2 );
  BOOST_CHECK_EQUAL( agreement_vec[0].cycles.value, 300 );
  BOOST_CHECK_EQUAL( agreement_vec[0].frequency_lock.value, 0 );
  BOOST_CHECK_EQUAL( agreement_vec[1].cycles.value, 310 );
  BOOST_CHECK_EQUAL( agreement_vec[1].frequency_lock.value, 200 );

  // Bogus does not have a result:
  BOOST_CHECK( result_vec[4].account_id == bogus_id );
  BOOST_CHECK( !result_vec[4].result.valid() );

} FC_LOG_AND_RETHROW() }

/*BOOST_AUTO_TEST_CASE( get_all_cycle_balances_for_accounts_unit_test )
{ try {
  VAULT_ACTORS((first)(second)(third)(fourth))

  auto vec = _dal.get_all_cycle_balances_for_accounts({});

  BOOST_CHECK_EQUAL( vec.size(), 0 );

  adjust_cycles( first_id, 100 );
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), first_id, 110, 200, ""));

  adjust_cycles( second_id, 200 );

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), third_id, 300, 200, ""));

  vec = _dal.get_all_cycle_balances_for_accounts({first_id, second_id, third_id, fourth_id});

  BOOST_CHECK_EQUAL( vec.size(), 4 );

  auto cycle_vec = vec[0];
  BOOST_CHECK_EQUAL( cycle_vec.size(), 2 );
  BOOST_CHECK_EQUAL( cycle_vec[0].cycles.value, 100 );
  BOOST_CHECK_EQUAL( cycle_vec[0].frequency_lock.value, 0 );
  BOOST_CHECK_EQUAL( cycle_vec[1].cycles.value, 110 );
  BOOST_CHECK_EQUAL( cycle_vec[1].frequency_lock.value, 200 );

  cycle_vec = vec[1];
  BOOST_CHECK_EQUAL( cycle_vec.size(), 1 );
  BOOST_CHECK_EQUAL( cycle_vec[0].cycles.value, 200 );

  cycle_vec = vec[2];
  BOOST_CHECK_EQUAL( cycle_vec.size(), 2 );
  BOOST_CHECK_EQUAL( cycle_vec[0].cycles.value, 0 );
  BOOST_CHECK_EQUAL( cycle_vec[0].frequency_lock.value, 0 );
  BOOST_CHECK_EQUAL( cycle_vec[1].cycles.value, 300 );
  BOOST_CHECK_EQUAL( cycle_vec[1].frequency_lock.value, 200 );

  cycle_vec = vec[3];
  BOOST_CHECK_EQUAL( cycle_vec.size(), 1 );
  BOOST_CHECK_EQUAL( cycle_vec[0].cycles.value, 0 );
  BOOST_CHECK_EQUAL( cycle_vec[0].frequency_lock.value, 0 );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_dascoin_balances_for_accounts_unit_test )
{ try {
  VAULT_ACTORS((first)(second)(third)(fourth))

  auto das_vec = _dal.get_dascoin_balances_for_accounts({});

  BOOST_CHECK_EQUAL( das_vec.size(), 0 );

  db.issue_asset(first_id, 100 * DASCOIN_DEFAULT_ASSET_PRECISION, get_dascoin_asset_id(), 0);
  db.issue_asset(second_id, 200 * DASCOIN_DEFAULT_ASSET_PRECISION, get_dascoin_asset_id(), 0);
  db.issue_asset(third_id, 300 * DASCOIN_DEFAULT_ASSET_PRECISION, get_dascoin_asset_id(), 0);

  das_vec = _dal.get_dascoin_balances_for_accounts({first_id, second_id, third_id, fourth_id});

  BOOST_CHECK_EQUAL( das_vec.size(), 4 );
  BOOST_CHECK_EQUAL( das_vec[0].value, 100 * DASCOIN_DEFAULT_ASSET_PRECISION );
  BOOST_CHECK_EQUAL( das_vec[1].value, 200 * DASCOIN_DEFAULT_ASSET_PRECISION );
  BOOST_CHECK_EQUAL( das_vec[2].value, 300 * DASCOIN_DEFAULT_ASSET_PRECISION );
  BOOST_CHECK_EQUAL( das_vec[3].value, 0 );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_queue_submissions_with_pos_unit_test )
{ try {
  VAULT_ACTORS((first)(second)(third)(fourth))

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), first_id, 100, 200, "test1"));
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), first_id, 120, 200, ""));

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), second_id, 200, 200, ""));
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), second_id, 210, 200, ""));
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), second_id, 220, 200, ""));

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), third_id, 300, 200, ""));

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), fourth_id, 400, 200, ""));

  auto pos_vec = _dal.get_queue_submissions_with_pos(first_id);
  BOOST_CHECK_EQUAL( pos_vec.size(), 2 );

  uint32_t pos;
  reward_queue_object rqo;
  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 0 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == first_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 100 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );
  // BOOST_CHECK( rqo.time == db.head_block_time() ); // TODO: this fails for some reason?
  BOOST_CHECK( rqo.comment == "test1" );

  std::tie(pos, rqo) = pos_vec[1];
  BOOST_CHECK_EQUAL( pos, 1 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == first_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 120 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  pos_vec = _dal.get_queue_submissions_with_pos(second_id);
  BOOST_CHECK_EQUAL( pos_vec.size(), 3 );

  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 2 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == second_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 200 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  std::tie(pos, rqo) = pos_vec[1];
  BOOST_CHECK_EQUAL( pos, 3 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == second_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 210 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  std::tie(pos, rqo) = pos_vec[2];
  BOOST_CHECK_EQUAL( pos, 4 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == second_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 220 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  pos_vec = _dal.get_queue_submissions_with_pos(third_id);
  BOOST_CHECK_EQUAL( pos_vec.size(), 1 );

  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 5 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == third_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 300 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  pos_vec = _dal.get_queue_submissions_with_pos(fourth_id);
  BOOST_CHECK_EQUAL( pos_vec.size(), 1 );

  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 6 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == fourth_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 400 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_CASE( get_queue_submissions_with_pos_for_accounts_unit_test )
{ try {
  VAULT_ACTORS((first)(second)(third)(fourth))
  uint32_t pos;
  reward_queue_object rqo;

  auto vec = _dal.get_queue_submissions_with_pos_for_accounts({});
  BOOST_CHECK_EQUAL( vec.size(), 0 );

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), first_id, 100, 200, ""));

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), second_id, 200, 200, ""));
  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), second_id, 210, 200, ""));

  do_op(submit_reserve_cycles_to_queue_operation(get_cycle_issuer_id(), third_id, 300, 200, ""));

  vec = _dal.get_queue_submissions_with_pos_for_accounts({first_id, second_id, third_id, fourth_id});
  BOOST_CHECK_EQUAL( vec.size(), 4 );

  auto pos_vec = vec[0];
  BOOST_CHECK_EQUAL( pos_vec.size(), 1 );

  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 0 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == first_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 100 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );
  // BOOST_CHECK( rqo.time == db.head_block_time() ); // TODO: this fails for some reason?

  pos_vec = vec[1];
  BOOST_CHECK_EQUAL( pos_vec.size(), 2 );

  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 1 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == second_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 200 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  std::tie(pos, rqo) = pos_vec[1];
  BOOST_CHECK_EQUAL( pos, 2 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == second_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 210 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  pos_vec = vec[2];
  BOOST_CHECK_EQUAL( pos_vec.size(), 1 );

  std::tie(pos, rqo) = pos_vec[0];
  BOOST_CHECK_EQUAL( pos, 3 );
  BOOST_CHECK_EQUAL( rqo.origin, "reserve_cycles" );
  BOOST_CHECK( !rqo.license.valid() );
  BOOST_CHECK( rqo.account == third_id );
  BOOST_CHECK_EQUAL( rqo.amount.value, 300 );
  BOOST_CHECK_EQUAL( rqo.frequency.value, 200 );

  pos_vec = vec[3];
  BOOST_CHECK_EQUAL( pos_vec.size(), 0 );

} FC_LOG_AND_RETHROW() }*/

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()