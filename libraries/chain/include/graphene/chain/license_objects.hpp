/**
 * DASCOIN!
 */

#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/types.hpp>
#include <graphene/chain/upgrade_type.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/db/object.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace graphene { namespace chain {

  struct license_information
  {
    struct license_history_record
    {
      license_type_id_type license;
      share_type amount;
      frequency_type frequency_lock;
      time_point_sec activation_time;

      license_history_record() = default;
      license_history_record(license_type_id_type license, share_type amount, frequency_type frequency_lock, 
          time_point_sec activation_time) : 
            license(license), 
            amount(amount), 
            frequency_lock(frequency_lock), 
            activation_time(activation_time) {}
    };
    typedef vector<license_history_record> array_t;

    array_t history;
    optional<license_type_id_type> max_license;
    frequency_type frequency_lock;

    upgrade_type balance_upgrade;
    upgrade_type requeue_upgrade;
    upgrade_type return_upgrade;

    void add_license(license_type_id_type license_id, share_type amount, frequency_type f_lock,
                      time_point_sec activation_time)
    {
      history.emplace_back(license_id, amount, f_lock, activation_time);
      max_license = license_id;
      frequency_lock = f_lock;
    }
  };

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
      static const uint8_t space_id = protocol_ids;
      static const uint8_t type_id  = license_type_object_type;

      string name;                        // Name of the license.
      share_type amount = 0;              // The amount of cycles the license grants.

      license_kind kind = license_kind::regular;

      // All upgrades in the system:
      upgrade_type balance_upgrade;
      upgrade_type requeue_upgrade;
      upgrade_type return_upgrade;

      extensions_type extensions;

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

} }  // namespace graphene::chain

///////////////////////////////
// REFLECTIONS:              //
///////////////////////////////

FC_REFLECT( graphene::chain::license_information::license_history_record,
            (license)
            (amount)
            (frequency_lock)
            (activation_time)
          )

FC_REFLECT( graphene::chain::license_information,
            (history)
            (max_license)
            (frequency_lock)
            (balance_upgrade)
            (requeue_upgrade)
            (return_upgrade)
          )

FC_REFLECT_DERIVED( graphene::chain::license_type_object, (graphene::db::object),
                    (name)
                    (amount)
                    (kind)
                    (balance_upgrade)
                    (requeue_upgrade)
                    (return_upgrade)
                    (extensions)
                  )

