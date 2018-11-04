#pragma once
#include <eosio/chain/block_timestamp.hpp>
#include <eosio/chain/producer_schedule.hpp>

namespace eosio { namespace chain {

   struct block_header
   {
      block_timestamp_type             timestamp;   // 时间戳
      account_name                     producer;  // 见证人的账户

      /**
       *  By signing this block this producer is confirming blocks [block_num() - confirmed, blocknum()) 
       *  as being the best blocks for that range and that he has not signed any other
       *  statements that would contradict.  
       *
       *  No producer should sign a block with overlapping ranges or it is proof of byzantine
       *  behavior. When producing a block a producer is always confirming at least the block he
       *  is building off of.  A producer cannot confirm "this" block, only prior blocks.
       */
      uint16_t                         confirmed = 1;  

      block_id_type                    previous;       // 上一个区块的哈希

      checksum256_type                 transaction_mroot; /// mroot of cycles_summary    // 交易默克尔根，transaction 包含很多 action
      checksum256_type                 action_mroot; /// mroot of all delivered action receipts    // action 默克尔根，eos 里 action 是最小动作单位


      /** The producer schedule version that should validate this block, this is used to
       * indicate that the prior block which included new_producers->version has been marked
       * irreversible and that it the new producer schedule takes effect this block.
       */
      uint32_t                          schedule_version = 0;  // 见证人排序的编号。见证人的排序是见证人自己商议决定的
      optional<producer_schedule_type>  new_producers;      // 下一个见证人。用「optional」库表示可以为空，意思是还是当前见证人出块。
      extensions_type                   header_extensions;
 

      digest_type       digest()const;
      block_id_type     id() const;
      uint32_t          block_num() const { return num_from_id(previous) + 1; }
      static uint32_t   num_from_id(const block_id_type& id);
   };


   struct signed_block_header : public block_header    // 签名的区块头。不可篡改。签名表示认可区块头里的全部数据
   {
      signature_type    producer_signature;     // 见证人的签名
   };

   struct header_confirmation {
      block_id_type   block_id;
      account_name    producer;
      signature_type  producer_signature;
   };

} } /// namespace eosio::chain

FC_REFLECT(eosio::chain::block_header, 
           (timestamp)(producer)(confirmed)(previous)
           (transaction_mroot)(action_mroot)
           (schedule_version)(new_producers)(header_extensions))

FC_REFLECT_DERIVED(eosio::chain::signed_block_header, (eosio::chain::block_header), (producer_signature))
FC_REFLECT(eosio::chain::header_confirmation,  (block_id)(producer)(producer_signature) )
