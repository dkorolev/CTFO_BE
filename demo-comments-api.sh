#!/bin/bash

set -e

echo "CTFO comments API demo."

TEST_HOST=https://secure.c5t.io
# TEST_HOST=localhost:8383

TEST_DID=fake_did
TEST_KEY=fake_key

curl -s -X POST "$TEST_HOST/ctfo/auth/ios?id=$TEST_DID&key=$TEST_KEY" >auth.json
TEST_UID=$(cat auth.json | jq ".feed.user.uid" | xargs echo)
TEST_TOKEN=$(cat auth.json | jq ".feed.user.token" | xargs echo)

echo "CTFO user $TEST_UID, CTFO token $TEST_TOKEN"

echo "Adding a card."
cat <<EOF >add_card.json
{"card":{"text":"Foo."}}
EOF

echo -n "Request to POST /ctfo/card: "
cat add_card.json

echo -n "Response: "
curl -s -X POST --data @add_card.json "$TEST_HOST/ctfo/card?uid=$TEST_UID&token=$TEST_TOKEN" | tee my_card.json

TEST_MY_CID=$(cat my_card.json  | jq ".created.cid" | xargs echo)
echo "My card CID: $TEST_MY_CID"

echo -n "Comments to this card: "
curl -s "$TEST_HOST/ctfo/comments?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | jq ".comments.comments"

echo "Posting a comment."
cat <<EOF >add_comment.json
{"comment":{"text":"Bla."}}
EOF

echo -n "Request to POST /ctfo/comment: "
cat add_comment.json

echo -n "Response: "
curl -s -X POST --data @add_comment.json "$TEST_HOST/ctfo/comment?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | tee my_comment.json

echo -n "Comments to this card: "
curl -s "$TEST_HOST/ctfo/comments?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | jq ".comments.comments"

echo "Posting another comment."
cat <<EOF >add_comment.json
{"comment":{"text":"Above bla."}}
EOF

echo -n "Request to POST /ctfo/comment: "
cat add_comment.json

echo -n "Response: "
curl -s -X POST --data @add_comment.json "$TEST_HOST/ctfo/comment?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | tee my_comment.json

echo -n "Comments to this card: "
curl -s "$TEST_HOST/ctfo/comments?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | tee my_comments.json | jq ".comments.comments"

TEST_MY_OID=$(cat my_comments.json  | jq ".comments.comments[0].oid" | xargs echo)
echo "My comment OID: $TEST_MY_OID"

echo "Posting subcomment 1/2."
cat <<EOF >add_comment.json
{"comment":{"text":"foo","parent_oid":"$TEST_MY_OID"}}
EOF

echo -n "Request to POST /ctfo/comment: "
cat add_comment.json

echo -n "Response: "
curl -s -X POST --data @add_comment.json "$TEST_HOST/ctfo/comment?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | tee my_comment.json

echo "Posting subcomment 2/2."
cat <<EOF >add_comment.json
{"comment":{"text":"bar","parent_oid":"$TEST_MY_OID"}}
EOF

echo -n "Request to POST /ctfo/comment: "
cat add_comment.json

echo -n "Response: "
curl -s -X POST --data @add_comment.json "$TEST_HOST/ctfo/comment?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | tee my_comment.json

echo -n "Comments to this card: "
curl -s "$TEST_HOST/ctfo/comments?uid=$TEST_UID&token=$TEST_TOKEN&cid=$TEST_MY_CID" | tee my_comments.json | jq ".comments.comments"
