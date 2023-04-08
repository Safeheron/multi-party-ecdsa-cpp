
#include "message.h"

#include <google/protobuf/util/json_util.h>
#include "crypto-encode/base64.h"

using std::string;
using safeheron::bignum::BN;
using google::protobuf::util::Status;
using google::protobuf::util::MessageToJsonString;
using google::protobuf::util::JsonStringToMessage;
using google::protobuf::util::JsonPrintOptions;
using google::protobuf::util::JsonParseOptions;


namespace safeheron {
namespace multi_party_ecdsa{
namespace gg18{
namespace sign{

bool Round7BCMessage::ToProtoObject(safeheron::proto::multi_party_ecdsa::gg18::sign::Round7BCMessage &message) const {
    bool ok = true;

    safeheron::proto::CurvePoint point;
    ok = U_.ToProtoObject(point);
    if (!ok) return false;
    message.mutable_u()->CopyFrom(point);

    ok = T_.ToProtoObject(point);
    if (!ok) return false;
    message.mutable_t()->CopyFrom(point);

    string str;
    blind_factor_.ToHexStr(str);
    message.set_blind_factor(str);

    return true;
}

bool Round7BCMessage::FromProtoObject(const safeheron::proto::multi_party_ecdsa::gg18::sign::Round7BCMessage &message) {
    bool ok = true;

    ok = U_.FromProtoObject(message.u());
    if (!ok) return false;

    ok = T_.FromProtoObject(message.t());
    if (!ok) return false;

    blind_factor_ = BN::FromHexStr(message. blind_factor());

    return true;
}


typedef Round7BCMessage TheClass;
typedef safeheron::proto::multi_party_ecdsa::gg18::sign::Round7BCMessage ProtoObject;

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
