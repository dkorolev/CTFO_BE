#!/bin/bash

set -e

echo "CTFO cards API demo."

TEST_HOST=https://ctfo.current.ai
# TEST_HOST=https://secure.c5t.io
# TEST_HOST=localhost:8383

TEST_DID=fake_did
TEST_KEY=fake_key

curl -s -X POST "$TEST_HOST/ctfo/auth/ios?id=$TEST_DID&key=$TEST_KEY" >auth.json
TEST_UID=$(cat auth.json | jq ".feed.user.uid" | xargs echo)
TEST_TOKEN=$(cat auth.json | jq ".feed.user.token" | xargs echo)

echo "CTFO user $TEST_UID, CTFO token $TEST_TOKEN"

echo "My cards, should be zero."
curl -s "$TEST_HOST/ctfo/my_cards?uid=$TEST_UID&token=$TEST_TOKEN" >my_cards.json
cat my_cards.json | jq .

echo "Favorires, should be zero."
curl -s "$TEST_HOST/ctfo/favs?uid=$TEST_UID&token=$TEST_TOKEN" | jq .

echo "Adding a card."
cat <<EOF >add_card.json
{"card":{"text":"Foo.","color":{"red":1,"green":2,"blue":3}}}
EOF

echo -n "Request: "
cat add_card.json

echo -n "Response: "
curl -X POST --data @add_card.json "$TEST_HOST/ctfo/card?uid=$TEST_UID&token=$TEST_TOKEN"

echo "My cards, should be one."
curl -s "$TEST_HOST/ctfo/my_cards?uid=$TEST_UID&token=$TEST_TOKEN" >my_cards.json
cat my_cards.json | jq .

echo "Favorires, should be zero."
curl -s "$TEST_HOST/ctfo/favs?uid=$TEST_UID&token=$TEST_TOKEN" >favs.json
cat favs.json | jq .

TEST_MY_CID=$(cat my_cards.json  | jq ".my_cards.cards[0].cid" | xargs echo)
echo "My card CID: $TEST_MY_CID"

echo "FAV this card."
cat <<EOF >fav_my_card.json
{"iOSGenericEvent":{"customer_id":"","user_ms":0,"client_id":"","device_id":"$TEST_DID","event":"FAV","source":"Linux Terminal","fields":{"cid":"$TEST_MY_CID","token":"$TEST_TOKEN","uid":"$TEST_UID"},"unparsable_fields":[]},"":"T9202274666560225185"}
EOF

curl -X POST --data @fav_my_card.json "$TEST_HOST/ctfo/log"

echo "CTFO on this card."
cat <<EOF >ctfo_my_card.json
{"iOSGenericEvent":{"customer_id":"","user_ms":0,"client_id":"","device_id":"$TEST_DID","event":"CTFO","source":"Linux Terminal","fields":{"cid":"$TEST_MY_CID","token":"$TEST_TOKEN","uid":"$TEST_UID"},"unparsable_fields":[]},"":"T9202274666560225185"}
EOF

curl -X POST --data @ctfo_my_card.json "$TEST_HOST/ctfo/log"

echo "GET this card."
curl -s "$TEST_HOST/ctfo/card?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" >get_card.json
cat get_card.json | jq .

echo "Favorires, should be one."
curl -s "$TEST_HOST/ctfo/favs?uid=$TEST_UID&token=$TEST_TOKEN" >favs.json
cat favs.json | jq .

echo "UNFAV this card."
cat <<EOF >unfav_my_card.json
{"iOSGenericEvent":{"customer_id":"","user_ms":0,"client_id":"","device_id":"$TEST_DID","event":"UNFAV","source":"Linux Terminal","fields":{"cid":"$TEST_MY_CID","token":"$TEST_TOKEN","uid":"$TEST_UID"},"unparsable_fields":[]},"":"T9202274666560225185"}
EOF

curl -X POST --data @unfav_my_card.json "$TEST_HOST/ctfo/log"

echo "My cards, should be one."
curl -s "$TEST_HOST/ctfo/my_cards?uid=$TEST_UID&token=$TEST_TOKEN" >my_cards.json
cat my_cards.json | jq .

echo "Favorires, should be zero."
curl -s "$TEST_HOST/ctfo/favs?uid=$TEST_UID&token=$TEST_TOKEN" >favs.json
cat favs.json | jq .
