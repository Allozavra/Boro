// Copyright 2020-2022 The MathWorks, Inc.
// Common copy functions for boro_interfaces/BowTarget
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#pragma warning(disable : 4265)
#pragma warning(disable : 4456)
#pragma warning(disable : 4458)
#pragma warning(disable : 4946)
#pragma warning(disable : 4244)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Wdelete-non-virtual-dtor"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif //_MSC_VER
#include "rclcpp/rclcpp.hpp"
#include "boro_interfaces/msg/bow_target.hpp"
#include "visibility_control.h"
#include "class_loader/multi_library_class_loader.hpp"
#include "ROS2PubSubTemplates.hpp"
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_msg_BowTarget_common : public MATLABROS2MsgInterface<boro_interfaces::msg::BowTarget> {
  public:
    virtual ~ros2_boro_interfaces_msg_BowTarget_common(){}
    virtual void copy_from_struct(boro_interfaces::msg::BowTarget* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const boro_interfaces::msg::BowTarget* msg, MultiLibLoader loader, size_t size = 1);
};
  void ros2_boro_interfaces_msg_BowTarget_common::copy_from_struct(boro_interfaces::msg::BowTarget* msg, const matlab::data::Struct& arr,
               MultiLibLoader loader) {
    try {
        //header
        const matlab::data::StructArray header_arr = arr["header"];
        auto msgClassPtr_header = getCommonObject<std_msgs::msg::Header>("ros2_std_msgs_msg_Header_common",loader);
        msgClassPtr_header->copy_from_struct(&msg->header,header_arr[0],loader);
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'header' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'header' is wrong type; expected a struct.");
    }
    try {
        //alpha_mode
        const matlab::data::TypedArray<uint8_t> alpha_mode_arr = arr["alpha_mode"];
        msg->alpha_mode = alpha_mode_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'alpha_mode' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'alpha_mode' is wrong type; expected a uint8.");
    }
    try {
        //h_mode
        const matlab::data::TypedArray<uint8_t> h_mode_arr = arr["h_mode"];
        msg->h_mode = h_mode_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_mode' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_mode' is wrong type; expected a uint8.");
    }
    try {
        //l_mode
        const matlab::data::TypedArray<uint8_t> l_mode_arr = arr["l_mode"];
        msg->l_mode = l_mode_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'l_mode' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'l_mode' is wrong type; expected a uint8.");
    }
    try {
        //alpha_target
        const matlab::data::TypedArray<double> alpha_target_arr = arr["alpha_target"];
        msg->alpha_target = alpha_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'alpha_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'alpha_target' is wrong type; expected a double.");
    }
    try {
        //alpha_velocity_target
        const matlab::data::TypedArray<double> alpha_velocity_target_arr = arr["alpha_velocity_target"];
        msg->alpha_velocity_target = alpha_velocity_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'alpha_velocity_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'alpha_velocity_target' is wrong type; expected a double.");
    }
    try {
        //alpha_acceleration_target
        const matlab::data::TypedArray<double> alpha_acceleration_target_arr = arr["alpha_acceleration_target"];
        msg->alpha_acceleration_target = alpha_acceleration_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'alpha_acceleration_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'alpha_acceleration_target' is wrong type; expected a double.");
    }
    try {
        //h_target
        const matlab::data::TypedArray<double> h_target_arr = arr["h_target"];
        msg->h_target = h_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_target' is wrong type; expected a double.");
    }
    try {
        //h_velocity_target
        const matlab::data::TypedArray<double> h_velocity_target_arr = arr["h_velocity_target"];
        msg->h_velocity_target = h_velocity_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_velocity_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_velocity_target' is wrong type; expected a double.");
    }
    try {
        //h_acceleration_target
        const matlab::data::TypedArray<double> h_acceleration_target_arr = arr["h_acceleration_target"];
        msg->h_acceleration_target = h_acceleration_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_acceleration_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_acceleration_target' is wrong type; expected a double.");
    }
    try {
        //force_target
        const matlab::data::TypedArray<double> force_target_arr = arr["force_target"];
        msg->force_target = force_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'force_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'force_target' is wrong type; expected a double.");
    }
    try {
        //l_target
        const matlab::data::TypedArray<double> l_target_arr = arr["l_target"];
        msg->l_target = l_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'l_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'l_target' is wrong type; expected a double.");
    }
    try {
        //l_velocity_target
        const matlab::data::TypedArray<double> l_velocity_target_arr = arr["l_velocity_target"];
        msg->l_velocity_target = l_velocity_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'l_velocity_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'l_velocity_target' is wrong type; expected a double.");
    }
    try {
        //l_acceleration_target
        const matlab::data::TypedArray<double> l_acceleration_target_arr = arr["l_acceleration_target"];
        msg->l_acceleration_target = l_acceleration_target_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'l_acceleration_target' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'l_acceleration_target' is wrong type; expected a double.");
    }
    try {
        //enable
        const matlab::data::TypedArray<bool> enable_arr = arr["enable"];
        msg->enable = enable_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'enable' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'enable' is wrong type; expected a logical.");
    }
    try {
        //emergency_stop
        const matlab::data::TypedArray<bool> emergency_stop_arr = arr["emergency_stop"];
        msg->emergency_stop = emergency_stop_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'emergency_stop' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'emergency_stop' is wrong type; expected a logical.");
    }
    try {
        //source_state
        const matlab::data::CharArray source_state_arr = arr["source_state"];
        msg->source_state = source_state_arr.toAscii();
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'source_state' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'source_state' is wrong type; expected a string.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T ros2_boro_interfaces_msg_BowTarget_common::get_arr(MDFactory_T& factory, const boro_interfaces::msg::BowTarget* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","header","MODE_OFF","MODE_POSITION","MODE_VELOCITY","MODE_FORCE","alpha_mode","h_mode","l_mode","alpha_target","alpha_velocity_target","alpha_acceleration_target","h_target","h_velocity_target","h_acceleration_target","force_target","l_target","l_velocity_target","l_acceleration_target","enable","emergency_stop","source_state"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("boro_interfaces/BowTarget");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::msg::Header>("ros2_std_msgs_msg_Header_common",loader);
    outArray[ctr]["header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // MODE_OFF
    auto currentElement_MODE_OFF = (msg + ctr)->MODE_OFF;
    outArray[ctr]["MODE_OFF"] = factory.createScalar(currentElement_MODE_OFF);
    // MODE_POSITION
    auto currentElement_MODE_POSITION = (msg + ctr)->MODE_POSITION;
    outArray[ctr]["MODE_POSITION"] = factory.createScalar(currentElement_MODE_POSITION);
    // MODE_VELOCITY
    auto currentElement_MODE_VELOCITY = (msg + ctr)->MODE_VELOCITY;
    outArray[ctr]["MODE_VELOCITY"] = factory.createScalar(currentElement_MODE_VELOCITY);
    // MODE_FORCE
    auto currentElement_MODE_FORCE = (msg + ctr)->MODE_FORCE;
    outArray[ctr]["MODE_FORCE"] = factory.createScalar(currentElement_MODE_FORCE);
    // alpha_mode
    auto currentElement_alpha_mode = (msg + ctr)->alpha_mode;
    outArray[ctr]["alpha_mode"] = factory.createScalar(currentElement_alpha_mode);
    // h_mode
    auto currentElement_h_mode = (msg + ctr)->h_mode;
    outArray[ctr]["h_mode"] = factory.createScalar(currentElement_h_mode);
    // l_mode
    auto currentElement_l_mode = (msg + ctr)->l_mode;
    outArray[ctr]["l_mode"] = factory.createScalar(currentElement_l_mode);
    // alpha_target
    auto currentElement_alpha_target = (msg + ctr)->alpha_target;
    outArray[ctr]["alpha_target"] = factory.createScalar(currentElement_alpha_target);
    // alpha_velocity_target
    auto currentElement_alpha_velocity_target = (msg + ctr)->alpha_velocity_target;
    outArray[ctr]["alpha_velocity_target"] = factory.createScalar(currentElement_alpha_velocity_target);
    // alpha_acceleration_target
    auto currentElement_alpha_acceleration_target = (msg + ctr)->alpha_acceleration_target;
    outArray[ctr]["alpha_acceleration_target"] = factory.createScalar(currentElement_alpha_acceleration_target);
    // h_target
    auto currentElement_h_target = (msg + ctr)->h_target;
    outArray[ctr]["h_target"] = factory.createScalar(currentElement_h_target);
    // h_velocity_target
    auto currentElement_h_velocity_target = (msg + ctr)->h_velocity_target;
    outArray[ctr]["h_velocity_target"] = factory.createScalar(currentElement_h_velocity_target);
    // h_acceleration_target
    auto currentElement_h_acceleration_target = (msg + ctr)->h_acceleration_target;
    outArray[ctr]["h_acceleration_target"] = factory.createScalar(currentElement_h_acceleration_target);
    // force_target
    auto currentElement_force_target = (msg + ctr)->force_target;
    outArray[ctr]["force_target"] = factory.createScalar(currentElement_force_target);
    // l_target
    auto currentElement_l_target = (msg + ctr)->l_target;
    outArray[ctr]["l_target"] = factory.createScalar(currentElement_l_target);
    // l_velocity_target
    auto currentElement_l_velocity_target = (msg + ctr)->l_velocity_target;
    outArray[ctr]["l_velocity_target"] = factory.createScalar(currentElement_l_velocity_target);
    // l_acceleration_target
    auto currentElement_l_acceleration_target = (msg + ctr)->l_acceleration_target;
    outArray[ctr]["l_acceleration_target"] = factory.createScalar(currentElement_l_acceleration_target);
    // enable
    auto currentElement_enable = (msg + ctr)->enable;
    outArray[ctr]["enable"] = factory.createScalar(currentElement_enable);
    // emergency_stop
    auto currentElement_emergency_stop = (msg + ctr)->emergency_stop;
    outArray[ctr]["emergency_stop"] = factory.createScalar(currentElement_emergency_stop);
    // source_state
    auto currentElement_source_state = (msg + ctr)->source_state;
    outArray[ctr]["source_state"] = factory.createCharArray(currentElement_source_state);
    }
    return std::move(outArray);
  } 
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_BowTarget_message : public ROS2MsgElementInterfaceFactory {
  public:
    virtual ~ros2_boro_interfaces_BowTarget_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType /*type*/);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType /*type*/);
    virtual std::shared_ptr<void> generateCppMessage(ElementType /*type*/, const matlab::data::StructArray& /* arr */, MultiLibLoader /* loader */, std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
    virtual matlab::data::StructArray generateMLMessage(ElementType  /*type*/ ,void*  /* msg */, MultiLibLoader /* loader */ , std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          ros2_boro_interfaces_BowTarget_message::generatePublisherInterface(ElementType /*type*/){
    return std::make_shared<ROS2PublisherImpl<boro_interfaces::msg::BowTarget,ros2_boro_interfaces_msg_BowTarget_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         ros2_boro_interfaces_BowTarget_message::generateSubscriberInterface(ElementType /*type*/){
    return std::make_shared<ROS2SubscriberImpl<boro_interfaces::msg::BowTarget,ros2_boro_interfaces_msg_BowTarget_common>>();
  }
  std::shared_ptr<void> ros2_boro_interfaces_BowTarget_message::generateCppMessage(ElementType /*type*/, 
                                           const matlab::data::StructArray& arr,
                                           MultiLibLoader loader,
                                           std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* commonObjMap){
    auto msg = std::make_shared<boro_interfaces::msg::BowTarget>();
    ros2_boro_interfaces_msg_BowTarget_common commonObj;
    commonObj.mCommonObjMap = commonObjMap;
    commonObj.copy_from_struct(msg.get(), arr[0], loader);
    return msg;
  }
  matlab::data::StructArray ros2_boro_interfaces_BowTarget_message::generateMLMessage(ElementType  /*type*/ ,
                                                    void*  msg ,
                                                    MultiLibLoader  loader ,
                                                    std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>*  commonObjMap ){
    ros2_boro_interfaces_msg_BowTarget_common commonObj;	
    commonObj.mCommonObjMap = commonObjMap;	
    MDFactory_T factory;
    return commonObj.get_arr(factory, (boro_interfaces::msg::BowTarget*)msg, loader);			
 }
#include "class_loader/register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_msg_BowTarget_common, MATLABROS2MsgInterface<boro_interfaces::msg::BowTarget>)
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_BowTarget_message, ROS2MsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER