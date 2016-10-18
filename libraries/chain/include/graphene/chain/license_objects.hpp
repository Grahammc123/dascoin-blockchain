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
  // OBJECTS:                  //
  ///////////////////////////////

  /**
   * @class license_type_object
   * @brief The license type given to an account.
   * @ingroup object
   *
   * Contains a type of license that can be given to an account and the amount of cycles that the account
   * benefits from the license.
   */
  class license_type_object : public graphene::db::abstract_object<license_type_object>
  {
    public:
      static const uint8_t space_id = implementation_ids;
      static const uint8_t type_id  = impl_license_type_object_type;

      string name;                        // Name of the license.
      share_type amount = 0;              // The amount of cycles the license grants.

      // Cycle policy related:
      // TODO: add more stuff?
      uint8_t upgrades = 0;               // The number of cycle upgrades the license grants.
      uint32_t policy_flags = 0;
      uint8_t priority = 0;

      extensions_type extensions;
      // All bits must match the chartered pattern exactly.
      // TODO: this is broken!!!!!!
      bool is_chartered()const { return (CYCLE_POLICY_CHARTER_MASK == (policy_flags & CYCLE_POLICY_CHARTER_MASK)); }

      void validate() const;
  };

  inline bool operator == ( const license_type_object& lhs, const license_type_object& rhs )
  {
    return lhs.name == rhs.name;
  }
  inline bool operator < ( const license_type_object& lhs, const license_type_object& rhs )
  {
    return lhs.amount < rhs.amount;
  }
  inline bool operator <= ( const license_type_object& lhs, const license_type_object& rhs )
  {
    return lhs.amount <= rhs.amount;
  }
  inline bool operator > ( const license_type_object& lhs, const license_type_object& rhs )
  {
    return lhs.amount > rhs.amount;
  }
  inline bool operator >= ( const license_type_object& lhs, const license_type_object& rhs )
  {
    return lhs.amount >= rhs.amount;
  }

  /**
   * @class license_request_object
   * @brief A pending request to grant a license to an account.
   * @ingroup object
   *
   * This is an implementation detail.
   */
  class license_request_object : public graphene::db::abstract_object<license_request_object>
  {
    public:
      static const uint8_t space_id = implementation_ids;
      static const uint8_t type_id  = impl_license_request_object_type;

      account_id_type license_issuing_account;  // This MUST be the license authentication authority.
      account_id_type account;                  // The account to benefit the license.
      license_type_id_type license;             // The license to be granted to the account.
      time_point_sec expiration;                // Deadline for denial.
      optional<float> account_frequency;        // Account frequency lock to be applied.

      extensions_type extensions;

      void validate() const;
  };

  ///////////////////////////////
  // MULTI INDEX CONTAINERS:   //
  ///////////////////////////////

  struct by_name;
  struct by_amount;
  typedef multi_index_container<
     license_type_object,
     indexed_by<
        ordered_unique< tag<by_id>,
          member<object, object_id_type, &object::id> >,
        ordered_unique< tag<by_name>,
          member<license_type_object, string, &license_type_object::name>
        >,
        ordered_non_unique< tag<by_amount>,
          member<license_type_object, share_type, &license_type_object::amount>
        >
     >
  > license_type_multi_index_type;

  typedef generic_index<license_type_object, license_type_multi_index_type> license_type_index;

  struct by_account;
  struct by_expiration;
  struct by_license_type_id;
  struct by_issuer_id;
  typedef multi_index_container<
    license_request_object,
    indexed_by<
      ordered_unique< tag<by_id>,
        member< object, object_id_type, &object::id >
      >,
      ordered_unique< tag<by_account>,
        composite_key< license_request_object,
          member< license_request_object, account_id_type, &license_request_object::account >,
          member< object, object_id_type, &object::id>
        >
      >,
      ordered_unique< tag<by_expiration>,
        composite_key< license_request_object,
          member< license_request_object, time_point_sec, &license_request_object::expiration >,
          member< object, object_id_type, &object::id>
        >
      >,
      ordered_non_unique< tag<by_issuer_id>,
        member< license_request_object, account_id_type, &license_request_object::license_issuing_account >
      >,
      ordered_non_unique< tag<by_license_type_id>,
        member< license_request_object, license_type_id_type, &license_request_object::license >
      >
    >
  > license_request_multi_index_type;

  typedef generic_index<license_request_object, license_request_multi_index_type> license_request_index;

} }  // namespace graphene::chain

///////////////////////////////
// REFLECTIONS:              //
///////////////////////////////

FC_REFLECT_DERIVED( graphene::chain::license_type_object, (graphene::db::object),
                    (name)
                    (amount)
                    (upgrades)
                    (policy_flags)
                    (priority)
                    (extensions)
                  )

FC_REFLECT_DERIVED( graphene::chain::license_request_object, (graphene::db::object),
                    (license_issuing_account)
                    (account)
                    (license)
                    (expiration)
                    (account_frequency)
                    (extensions)
                  )
