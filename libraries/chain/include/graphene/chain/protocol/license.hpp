/**
 * DASCOIN!
 */

#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/types.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/db/object.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

  ///////////////////////////////
  // OPERATIONS:               //
  ///////////////////////////////

  /**
   * @brief create a license type object
   * @ingroup operations
   *
   * Create a new type of license. Must be signed by the current license_administration authority.
   */
  struct create_license_type_operation : public base_operation
  {
     struct fee_parameters_type {};  // No fees are paid for this operation.

     asset fee;
     account_id_type admin;

     string name;
     share_type amount;
     string kind;

     upgrade_multiplier_type balance_multipliers;
     upgrade_multiplier_type requeue_multipliers;
     upgrade_multiplier_type return_multipliers;

     account_id_type fee_payer() const { return admin; }
     void validate() const;
     share_type calculate_fee(const fee_parameters_type&) const { return 0; }
  };

  /**
   * @brief Request a license to be granted an account
   * @ingroup operations
   *
   * Grant a license to an account. This operation must be signed by the current license_issuer authority.
   */
  struct issue_license_operation : public base_operation
  {
    struct fee_parameters_type {};  // No fees are paid for this operation.

    asset fee;
    account_id_type issuer;

    account_id_type account;                  // The account to benefit the license.
    license_type_id_type license;             // The license to be granted.
    share_type bonus_percentage;              // The bonus multiplier of base license cycles.
    frequency_type frequency_lock;            // The frequency lock on this license, zero for none.

    extensions_type extensions;

    issue_license_operation() = default;
    explicit issue_license_operation(account_id_type i, account_id_type a, license_type_id_type l,
                                     share_type b_p, frequency_type f)
        : issuer(i), account(a), license(l), bonus_percentage(b_p), frequency_lock(f) {}

    account_id_type fee_payer() const { return issuer; }
    void validate() const;
    share_type calculate_fee(const fee_parameters_type&) const { return 0; }
  };

} }  // namespace graphene::chain

///////////////////////////////
// REFLECTIONS:              //
///////////////////////////////

FC_REFLECT( graphene::chain::create_license_type_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::create_license_type_operation,
            (fee)
            (admin)
            (name)
            (amount)
            (kind)
            (balance_multipliers)
            (requeue_multipliers)
            (return_multipliers)
          )

FC_REFLECT( graphene::chain::issue_license_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::issue_license_operation,
            (fee)
            (issuer)
            (account)
            (license)
            (bonus_percentage)
            (frequency_lock)
            (extensions)
          )

