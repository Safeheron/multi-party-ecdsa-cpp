
#include "message.h"

#include <google/protobuf/util/json_util.h>
#include "crypto-ecies/auth_enc.h"
#include "crypto-encode/base64.h"

using std::string;
using safeheron::bignum::BN;
using safeheron::ecies::AuthEnc;
using google::protobuf::util::Status;
using google::protobuf::util::MessageToJsonString;
using google::protobuf::util::JsonStringToMessage;
using google::protobuf::util::JsonPrintOptions;
using google::protobuf::util::JsonParseOptions;


namespace safeheron {
namespace multi_party_ecdsa{
namespace cmp{
namespace sign {


bool Round2P2PMessage::ToProtoObject(safeheron::proto::multi_party_ecdsa::cmp::sign::Round2P2PMessage &message) const {
    bool ok = true;

    string str;

    message.set_ssid(ssid_);

    index_.ToHexStr(str);
    message.set_index(str);

    delta_.ToHexStr(str);
    message.set_delta(str);

    safeheron::proto::CurvePoint point;
    ok = Delta_.ToProtoObject(point);
    if (!ok) return false;
    message.mutable_var_delta()->CopyFrom(point);

    safeheron::proto::PailEncGroupEleRangeProof t_proof_1;
    ok = psi_double_prime_ij_.ToProtoObject(t_proof_1);
    if (!ok) return false;
    message.mutable_psi_double_prime_ij()->CopyFrom(t_proof_1);

    return true;
}

bool Round2P2PMessage::FromProtoObject(const safeheron::proto::multi_party_ecdsa::cmp::sign::Round2P2PMessage &message) {
    bool ok = true;

    ssid_ = message.ssid();

    index_ = BN::FromHexStr(message.index());
    ok = (index_ != 0);
    if (!ok) return false;

    delta_ = BN::FromHexStr(message.delta());

    ok = Delta_.FromProtoObject(message.var_delta());
    if (!ok) return false;

    ok = psi_double_prime_ij_.FromProtoObject(message.psi_double_prime_ij());
    if (!ok) return false;

    return true;
}


typedef Round2P2PMessage TheClass;
typedef safeheron::proto::multi_party_ecdsa::cmp::sign::Round2P2PMessage ProtoObject;


bool TheClass::ToBase64(string &b64) const {
    bool ok = true;
    b64.clear();
    ProtoObject proto_object;
    ok = ToProtoObject(proto_object);
    if (!ok) return false;

    string proto_bin = proto_object.SerializeAsString();
    b64 = safeheron::encode::base64::EncodeToBase64(proto_bin, true);
    return true;
}

bool TheClass::FromBase64(const string &b64) {
    bool ok = true;

    string data = safeheron::encode::base64::DecodeFromBase64(b64);

    ProtoObject proto_object;
    ok = proto_object.ParseFromString(data);
    if (!ok) return false;

    return FromProtoObject(proto_object);
}

bool TheClass::ToJsonString(string &json_str) const {
    bool ok = true;
    json_str.clear();
    ProtoObject proto_object;
    ok = ToProtoObject(proto_object);
    if (!ok) return false;

    JsonPrintOptions jp_option;
    jp_option.add_whitespace = true;
    Status stat = MessageToJsonString(proto_object, &json_str, jp_option);
    if (!stat.ok()) return false;

    return true;
}


bool TheClass::FromJsonString(const string &json_str) {
    ProtoObject proto_object;
    google::protobuf::util::JsonParseOptions jp_option;
    jp_option.ignore_unknown_fields = true;
    Status stat = JsonStringToMessage(json_str, &proto_object);
    if (!stat.ok()) return false;

    return FromProtoObject(proto_object);
}

}
}
}
}
