/**
 * DASCOIN!
 */
#include <graphene/chain/account_object.hpp>
#include <graphene/chain/database.hpp>
#include <graphene/chain/personal_identity_evaluator.hpp>

namespace graphene { namespace chain {

void_result update_pi_limits_evaluator::do_evaluate(const update_pi_limits_operation& op)
{ try {

  const database& d = db();
  const auto gpo = d.get_global_properties();

  account_id_type pi_validator_id = gpo.authorities.pi_validator;
  const account_object& account = op.account(d);

  FC_ASSERT( pi_validator_id == op.pi_validator );  // This is signed by the current validator id.

  // TODO: check if the level is OK?

  if( op.new_limits.valid() )
  {
    auto& new_limits = *op.new_limits;
    FC_ASSERT( new_limits.size() < static_cast<uint8_t>(limit_kind::LIMIT_KIND_COUNT) );  // Must not have more fields than the set amount.

    auto min = gpo.parameters.minimum_transfer_limit;
    auto max = gpo.parameters.maximum_transfer_limit;
    for ( auto limit : new_limits )
    {
      FC_ASSERT( limit > min && limit < max );
    }
  }
  acnt = &account;

} FC_CAPTURE_AND_RETHROW((op)) }

void_result update_pi_limits_evaluator::do_apply(const update_pi_limits_operation& op)
{ try {
  // Update the levels and the limits on the account:
  db().modify(*acnt, [&](account_object& a) {
    a.pi_level = op.level;
    if ( op.new_limits.valid() )
      a.limits = *op.new_limits;
  });
  // TODO: for each balance, set the new limits.
  // TODO: for each wallet, update the wallet's level if the vault's level is greater.
} FC_CAPTURE_AND_RETHROW((op)) }

} }  // namespace graphene::chain

