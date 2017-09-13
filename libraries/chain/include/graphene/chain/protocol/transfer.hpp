/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/memo.hpp>

namespace graphene { namespace chain {

////////////////////////////////
// OPERATIONS:                //
////////////////////////////////

   /**
    * @ingroup operations
    *
    * @brief Transfers an amount of one asset from one account to another
    *
    *  Fees are paid by the "from" account
    *
    *  @pre amount.amount > 0
    *  @pre fee.amount >= 0
    *  @pre from != to
    *  @post from account's balance will be reduced by fee and amount
    *  @post to account's balance will be increased by amount
    *  @return n/a
    */
   struct transfer_operation : public base_operation
   {
      struct fee_parameters_type {
         uint64_t fee       = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
         uint32_t price_per_kbyte = 10 * GRAPHENE_BLOCKCHAIN_PRECISION; /// only required for large memos.
      };

      asset            fee;
      /// Account to transfer asset from
      account_id_type  from;
      /// Account to transfer asset to
      account_id_type  to;
      /// The amount of asset to transfer from @ref from to @ref to
      asset            amount;

      /// User provided data encrypted to the memo key of the "to" account
      optional<memo_data> memo;
      extensions_type   extensions;

      account_id_type fee_payer()const { return from; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& k)const;
   };

   /**
    * @class transfer_vault_to_wallet_operation
    * @brief Transfers assets from a tethered vault to its parent wallet, with limits enforced.
    * @ingroup operations
    */
   struct transfer_vault_to_wallet_operation : public base_operation
   {
      struct fee_parameters_type {};

      asset fee;

      account_id_type from_vault;
      account_id_type to_wallet;
      asset asset_to_transfer;
      share_type reserved_to_transfer;

      extensions_type   extensions;

      account_id_type fee_payer()const { return from_vault; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& k)const { return 0; }
   };

   /**
    * @class transfer_wallet_to_vault_operation
    * @brief Transfers assets from a tethered vault to its parent wallet. NO LIMITS are enforced.
    * @ingroup operations
    */
   struct transfer_wallet_to_vault_operation : public base_operation
   {
      struct fee_parameters_type {};

      asset fee;

      account_id_type from_wallet;
      account_id_type to_vault;
      asset asset_to_transfer;
      share_type reserved_to_transfer;

      extensions_type   extensions;

      transfer_wallet_to_vault_operation() = default;
      explicit transfer_wallet_to_vault_operation(account_id_type from_wallet, account_id_type to_vault, asset asset_to_transfer,
                                                  share_type reserved_to_transfer)
         : from_wallet(from_wallet),
           to_vault(to_vault),
           asset_to_transfer(asset_to_transfer),
           reserved_to_transfer(reserved_to_transfer)
         {}
      account_id_type fee_payer()const { return from_wallet; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& k)const { return 0; }
   };

   /**
    *  @class override_transfer_operation
    *  @brief Allows the issuer of an asset to transfer an asset from any account to any account if they have override_authority
    *  @ingroup operations
    *
    *  @pre amount.asset_id->issuer == issuer
    *  @pre issuer != from  because this is pointless, use a normal transfer operation
    */
   struct override_transfer_operation : public base_operation
   {
      struct fee_parameters_type {
         uint64_t fee       = 20 * GRAPHENE_BLOCKCHAIN_PRECISION;
         uint32_t price_per_kbyte = 10; /// only required for large memos.
      };

      asset           fee;
      account_id_type issuer;
      /// Account to transfer asset from
      account_id_type from;
      /// Account to transfer asset to
      account_id_type to;
      /// The amount of asset to transfer from @ref from to @ref to
      asset amount;

      /// User provided data encrypted to the memo key of the "to" account
      optional<memo_data> memo;
      extensions_type   extensions;

      account_id_type fee_payer()const { return issuer; }
      void            validate()const;
      share_type      calculate_fee(const fee_parameters_type& k)const;
   };

   /**
    * @class update_euro_limit_operation
    * @brief Allows the authority to disable or enable the euro limit to an account
    * @ingroup operations
    */
   struct update_euro_limit_operation : public base_operation
   {
       struct fee_parameters_type {};

       asset           fee;
       account_id_type authority;
       // Account to set or reset limit
       account_id_type account;
       // Disable or enable limit
       bool            disable_limit;
       // Optional amount of euro limit
       optional<share_type> eur_limit;
       // Comment
       string          comment;
       extensions_type extensions;

       account_id_type fee_payer()const { return authority; }
       void            validate()const;
       share_type      calculate_fee(const fee_parameters_type& k)const { return 0; }
   };
}} // graphene::chain

////////////////////////////////
// REFLECTIONS:               //
////////////////////////////////

// override_transfer_operation

FC_REFLECT( graphene::chain::override_transfer_operation::fee_parameters_type, (fee)(price_per_kbyte) )
FC_REFLECT( graphene::chain::override_transfer_operation, (fee)(issuer)(from)(to)(amount)(memo)(extensions) )

// transfer_operation:

FC_REFLECT( graphene::chain::transfer_operation::fee_parameters_type, (fee)(price_per_kbyte) )
FC_REFLECT( graphene::chain::transfer_operation, (fee)(from)(to)(amount)(memo)(extensions) )

// transfer_vault_to_wallet_operation:

FC_REFLECT( graphene::chain::transfer_vault_to_wallet_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::transfer_vault_to_wallet_operation,
            (fee)
            (from_vault)
            (to_wallet)
            (asset_to_transfer)
            (reserved_to_transfer)
            (extensions)
          )

// transfer_wallet_to_vault_operation:

FC_REFLECT( graphene::chain::transfer_wallet_to_vault_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::transfer_wallet_to_vault_operation,
            (fee)
            (from_wallet)
            (to_vault)
            (asset_to_transfer)
            (reserved_to_transfer)
            (extensions)
          )

// update_euro_limit_operation:
FC_REFLECT( graphene::chain::update_euro_limit_operation::fee_parameters_type, )
FC_REFLECT( graphene::chain::update_euro_limit_operation,
            (fee)
            (authority)
            (account)
            (disable_limit)
            (eur_limit)
            (comment)
            (extensions)
          )
