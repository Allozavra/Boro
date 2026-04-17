// Copyright 2020-2022 The MathWorks, Inc.
// Common copy functions for boro_interfaces/BowState
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
#include "boro_interfaces/msg/bow_state.hpp"
#include "visibility_control.h"
#include "class_loader/multi_library_class_loader.hpp"
#include "ROS2PubSubTemplates.hpp"
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_msg_BowState_common : public MATLABROS2MsgInterface<boro_interfaces::msg::BowState> {
  public:
    virtual ~ros2_boro_interfaces_msg_BowState_common(){}
    virtual void copy_from_struct(boro_interfaces::msg::BowState* msg, const matlab::data::Struct& arr, MultiLibLoader loader); 
    //----------------------------------------------------------------------------
    virtual MDArray_T get_arr(MDFactory_T& factory, const boro_interfaces::msg::BowState* msg, MultiLibLoader loader, size_t size = 1);
};
  void ros2_boro_interfaces_msg_BowState_common::copy_from_struct(boro_interfaces::msg::BowState* msg, const matlab::data::Struct& arr,
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
        //alpha
        const matlab::data::TypedArray<double> alpha_arr = arr["alpha"];
        msg->alpha = alpha_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'alpha' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'alpha' is wrong type; expected a double.");
    }
    try {
        //h
        const matlab::data::TypedArray<double> h_arr = arr["h"];
        msg->h = h_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h' is wrong type; expected a double.");
    }
    try {
        //l
        const matlab::data::TypedArray<double> l_arr = arr["l"];
        msg->l = l_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'l' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'l' is wrong type; expected a double.");
    }
    try {
        //alpha_dot
        const matlab::data::TypedArray<double> alpha_dot_arr = arr["alpha_dot"];
        msg->alpha_dot = alpha_dot_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'alpha_dot' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'alpha_dot' is wrong type; expected a double.");
    }
    try {
        //h_dot
        const matlab::data::TypedArray<double> h_dot_arr = arr["h_dot"];
        msg->h_dot = h_dot_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'h_dot' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'h_dot' is wrong type; expected a double.");
    }
    try {
        //l_dot
        const matlab::data::TypedArray<double> l_dot_arr = arr["l_dot"];
        msg->l_dot = l_dot_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'l_dot' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'l_dot' is wrong type; expected a double.");
    }
    try {
        //force
        const matlab::data::TypedArray<double> force_arr = arr["force"];
        msg->force = force_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'force' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'force' is wrong type; expected a double.");
    }
    try {
        //in_contact
        const matlab::data::TypedArray<bool> in_contact_arr = arr["in_contact"];
        msg->in_contact = in_contact_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'in_contact' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'in_contact' is wrong type; expected a logical.");
    }
    try {
        //controller_ready
        const matlab::data::TypedArray<bool> controller_ready_arr = arr["controller_ready"];
        msg->controller_ready = controller_ready_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'controller_ready' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'controller_ready' is wrong type; expected a logical.");
    }
    try {
        //target_received
        const matlab::data::TypedArray<bool> target_received_arr = arr["target_received"];
        msg->target_received = target_received_arr[0];
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'target_received' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'target_received' is wrong type; expected a logical.");
    }
    try {
        //active_state
        const matlab::data::CharArray active_state_arr = arr["active_state"];
        msg->active_state = active_state_arr.toAscii();
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'active_state' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'active_state' is wrong type; expected a string.");
    }
    try {
        //controller_mode
        const matlab::data::CharArray controller_mode_arr = arr["controller_mode"];
        msg->controller_mode = controller_mode_arr.toAscii();
    } catch (matlab::data::InvalidFieldNameException&) {
        throw std::invalid_argument("Field 'controller_mode' is missing.");
    } catch (matlab::Exception&) {
        throw std::invalid_argument("Field 'controller_mode' is wrong type; expected a string.");
    }
  }
  //----------------------------------------------------------------------------
  MDArray_T ros2_boro_interfaces_msg_BowState_common::get_arr(MDFactory_T& factory, const boro_interfaces::msg::BowState* msg,
       MultiLibLoader loader, size_t size) {
    auto outArray = factory.createStructArray({size,1},{"MessageType","header","alpha","h","l","alpha_dot","h_dot","l_dot","force","in_contact","controller_ready","target_received","active_state","controller_mode"});
    for(size_t ctr = 0; ctr < size; ctr++){
    outArray[ctr]["MessageType"] = factory.createCharArray("boro_interfaces/BowState");
    // header
    auto currentElement_header = (msg + ctr)->header;
    auto msgClassPtr_header = getCommonObject<std_msgs::msg::Header>("ros2_std_msgs_msg_Header_common",loader);
    outArray[ctr]["header"] = msgClassPtr_header->get_arr(factory, &currentElement_header, loader);
    // alpha
    auto currentElement_alpha = (msg + ctr)->alpha;
    outArray[ctr]["alpha"] = factory.createScalar(currentElement_alpha);
    // h
    auto currentElement_h = (msg + ctr)->h;
    outArray[ctr]["h"] = factory.createScalar(currentElement_h);
    // l
    auto currentElement_l = (msg + ctr)->l;
    outArray[ctr]["l"] = factory.createScalar(currentElement_l);
    // alpha_dot
    auto currentElement_alpha_dot = (msg + ctr)->alpha_dot;
    outArray[ctr]["alpha_dot"] = factory.createScalar(currentElement_alpha_dot);
    // h_dot
    auto currentElement_h_dot = (msg + ctr)->h_dot;
    outArray[ctr]["h_dot"] = factory.createScalar(currentElement_h_dot);
    // l_dot
    auto currentElement_l_dot = (msg + ctr)->l_dot;
    outArray[ctr]["l_dot"] = factory.createScalar(currentElement_l_dot);
    // force
    auto currentElement_force = (msg + ctr)->force;
    outArray[ctr]["force"] = factory.createScalar(currentElement_force);
    // in_contact
    auto currentElement_in_contact = (msg + ctr)->in_contact;
    outArray[ctr]["in_contact"] = factory.createScalar(currentElement_in_contact);
    // controller_ready
    auto currentElement_controller_ready = (msg + ctr)->controller_ready;
    outArray[ctr]["controller_ready"] = factory.createScalar(currentElement_controller_ready);
    // target_received
    auto currentElement_target_received = (msg + ctr)->target_received;
    outArray[ctr]["target_received"] = factory.createScalar(currentElement_target_received);
    // active_state
    auto currentElement_active_state = (msg + ctr)->active_state;
    outArray[ctr]["active_state"] = factory.createCharArray(currentElement_active_state);
    // controller_mode
    auto currentElement_controller_mode = (msg + ctr)->controller_mode;
    outArray[ctr]["controller_mode"] = factory.createCharArray(currentElement_controller_mode);
    }
    return std::move(outArray);
  } 
class BORO_INTERFACES_EXPORT ros2_boro_interfaces_BowState_message : public ROS2MsgElementInterfaceFactory {
  public:
    virtual ~ros2_boro_interfaces_BowState_message(){}
    virtual std::shared_ptr<MATLABPublisherInterface> generatePublisherInterface(ElementType /*type*/);
    virtual std::shared_ptr<MATLABSubscriberInterface> generateSubscriberInterface(ElementType /*type*/);
    virtual std::shared_ptr<void> generateCppMessage(ElementType /*type*/, const matlab::data::StructArray& /* arr */, MultiLibLoader /* loader */, std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
    virtual matlab::data::StructArray generateMLMessage(ElementType  /*type*/ ,void*  /* msg */, MultiLibLoader /* loader */ , std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* /*commonObjMap*/);
};  
  std::shared_ptr<MATLABPublisherInterface> 
          ros2_boro_interfaces_BowState_message::generatePublisherInterface(ElementType /*type*/){
    return std::make_shared<ROS2PublisherImpl<boro_interfaces::msg::BowState,ros2_boro_interfaces_msg_BowState_common>>();
  }
  std::shared_ptr<MATLABSubscriberInterface> 
         ros2_boro_interfaces_BowState_message::generateSubscriberInterface(ElementType /*type*/){
    return std::make_shared<ROS2SubscriberImpl<boro_interfaces::msg::BowState,ros2_boro_interfaces_msg_BowState_common>>();
  }
  std::shared_ptr<void> ros2_boro_interfaces_BowState_message::generateCppMessage(ElementType /*type*/, 
                                           const matlab::data::StructArray& arr,
                                           MultiLibLoader loader,
                                           std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>* commonObjMap){
    auto msg = std::make_shared<boro_interfaces::msg::BowState>();
    ros2_boro_interfaces_msg_BowState_common commonObj;
    commonObj.mCommonObjMap = commonObjMap;
    commonObj.copy_from_struct(msg.get(), arr[0], loader);
    return msg;
  }
  matlab::data::StructArray ros2_boro_interfaces_BowState_message::generateMLMessage(ElementType  /*type*/ ,
                                                    void*  msg ,
                                                    MultiLibLoader  loader ,
                                                    std::map<std::string,std::shared_ptr<MATLABROS2MsgInterfaceBase>>*  commonObjMap ){
    ros2_boro_interfaces_msg_BowState_common commonObj;	
    commonObj.mCommonObjMap = commonObjMap;	
    MDFactory_T factory;
    return commonObj.get_arr(factory, (boro_interfaces::msg::BowState*)msg, loader);			
 }
#include "class_loader/register_macro.hpp"
// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_msg_BowState_common, MATLABROS2MsgInterface<boro_interfaces::msg::BowState>)
CLASS_LOADER_REGISTER_CLASS(ros2_boro_interfaces_BowState_message, ROS2MsgElementInterfaceFactory)
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif //_MSC_VER