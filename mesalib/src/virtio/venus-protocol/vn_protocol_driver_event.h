/* This file is generated by venus-protocol.  See vn_protocol_driver.h. */

/*
 * Copyright 2020 Google LLC
 * SPDX-License-Identifier: MIT
 */

#ifndef VN_PROTOCOL_DRIVER_EVENT_H
#define VN_PROTOCOL_DRIVER_EVENT_H

#include "vn_instance.h"
#include "vn_protocol_driver_structs.h"

/* struct VkEventCreateInfo chain */

static inline size_t
vn_sizeof_VkEventCreateInfo_pnext(const void *val)
{
    /* no known/supported struct */
    return vn_sizeof_simple_pointer(NULL);
}

static inline size_t
vn_sizeof_VkEventCreateInfo_self(const VkEventCreateInfo *val)
{
    size_t size = 0;
    /* skip val->{sType,pNext} */
    size += vn_sizeof_VkFlags(&val->flags);
    return size;
}

static inline size_t
vn_sizeof_VkEventCreateInfo(const VkEventCreateInfo *val)
{
    size_t size = 0;

    size += vn_sizeof_VkStructureType(&val->sType);
    size += vn_sizeof_VkEventCreateInfo_pnext(val->pNext);
    size += vn_sizeof_VkEventCreateInfo_self(val);

    return size;
}

static inline void
vn_encode_VkEventCreateInfo_pnext(struct vn_cs_encoder *enc, const void *val)
{
    /* no known/supported struct */
    vn_encode_simple_pointer(enc, NULL);
}

static inline void
vn_encode_VkEventCreateInfo_self(struct vn_cs_encoder *enc, const VkEventCreateInfo *val)
{
    /* skip val->{sType,pNext} */
    vn_encode_VkFlags(enc, &val->flags);
}

static inline void
vn_encode_VkEventCreateInfo(struct vn_cs_encoder *enc, const VkEventCreateInfo *val)
{
    assert(val->sType == VK_STRUCTURE_TYPE_EVENT_CREATE_INFO);
    vn_encode_VkStructureType(enc, &(VkStructureType){ VK_STRUCTURE_TYPE_EVENT_CREATE_INFO });
    vn_encode_VkEventCreateInfo_pnext(enc, val->pNext);
    vn_encode_VkEventCreateInfo_self(enc, val);
}

static inline size_t vn_sizeof_vkCreateEvent(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkCreateEvent_EXT;
    const VkFlags cmd_flags = 0;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type) + vn_sizeof_VkFlags(&cmd_flags);

    cmd_size += vn_sizeof_VkDevice(&device);
    cmd_size += vn_sizeof_simple_pointer(pCreateInfo);
    if (pCreateInfo)
        cmd_size += vn_sizeof_VkEventCreateInfo(pCreateInfo);
    cmd_size += vn_sizeof_simple_pointer(pAllocator);
    if (pAllocator)
        assert(false);
    cmd_size += vn_sizeof_simple_pointer(pEvent);
    if (pEvent)
        cmd_size += vn_sizeof_VkEvent(pEvent);

    return cmd_size;
}

static inline void vn_encode_vkCreateEvent(struct vn_cs_encoder *enc, VkCommandFlagsEXT cmd_flags, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkCreateEvent_EXT;

    vn_encode_VkCommandTypeEXT(enc, &cmd_type);
    vn_encode_VkFlags(enc, &cmd_flags);

    vn_encode_VkDevice(enc, &device);
    if (vn_encode_simple_pointer(enc, pCreateInfo))
        vn_encode_VkEventCreateInfo(enc, pCreateInfo);
    if (vn_encode_simple_pointer(enc, pAllocator))
        assert(false);
    if (vn_encode_simple_pointer(enc, pEvent))
        vn_encode_VkEvent(enc, pEvent);
}

static inline size_t vn_sizeof_vkCreateEvent_reply(VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkCreateEvent_EXT;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type);

    VkResult ret;
    cmd_size += vn_sizeof_VkResult(&ret);
    /* skip device */
    /* skip pCreateInfo */
    /* skip pAllocator */
    cmd_size += vn_sizeof_simple_pointer(pEvent);
    if (pEvent)
        cmd_size += vn_sizeof_VkEvent(pEvent);

    return cmd_size;
}

static inline VkResult vn_decode_vkCreateEvent_reply(struct vn_cs_decoder *dec, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    VkCommandTypeEXT command_type;
    vn_decode_VkCommandTypeEXT(dec, &command_type);
    assert(command_type == VK_COMMAND_TYPE_vkCreateEvent_EXT);

    VkResult ret;
    vn_decode_VkResult(dec, &ret);
    /* skip device */
    /* skip pCreateInfo */
    /* skip pAllocator */
    if (vn_decode_simple_pointer(dec)) {
        vn_decode_VkEvent(dec, pEvent);
    } else {
        pEvent = NULL;
    }

    return ret;
}

static inline size_t vn_sizeof_vkDestroyEvent(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkDestroyEvent_EXT;
    const VkFlags cmd_flags = 0;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type) + vn_sizeof_VkFlags(&cmd_flags);

    cmd_size += vn_sizeof_VkDevice(&device);
    cmd_size += vn_sizeof_VkEvent(&event);
    cmd_size += vn_sizeof_simple_pointer(pAllocator);
    if (pAllocator)
        assert(false);

    return cmd_size;
}

static inline void vn_encode_vkDestroyEvent(struct vn_cs_encoder *enc, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkDestroyEvent_EXT;

    vn_encode_VkCommandTypeEXT(enc, &cmd_type);
    vn_encode_VkFlags(enc, &cmd_flags);

    vn_encode_VkDevice(enc, &device);
    vn_encode_VkEvent(enc, &event);
    if (vn_encode_simple_pointer(enc, pAllocator))
        assert(false);
}

static inline size_t vn_sizeof_vkDestroyEvent_reply(VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkDestroyEvent_EXT;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type);

    /* skip device */
    /* skip event */
    /* skip pAllocator */

    return cmd_size;
}

static inline void vn_decode_vkDestroyEvent_reply(struct vn_cs_decoder *dec, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    VkCommandTypeEXT command_type;
    vn_decode_VkCommandTypeEXT(dec, &command_type);
    assert(command_type == VK_COMMAND_TYPE_vkDestroyEvent_EXT);

    /* skip device */
    /* skip event */
    /* skip pAllocator */
}

static inline size_t vn_sizeof_vkGetEventStatus(VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkGetEventStatus_EXT;
    const VkFlags cmd_flags = 0;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type) + vn_sizeof_VkFlags(&cmd_flags);

    cmd_size += vn_sizeof_VkDevice(&device);
    cmd_size += vn_sizeof_VkEvent(&event);

    return cmd_size;
}

static inline void vn_encode_vkGetEventStatus(struct vn_cs_encoder *enc, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkGetEventStatus_EXT;

    vn_encode_VkCommandTypeEXT(enc, &cmd_type);
    vn_encode_VkFlags(enc, &cmd_flags);

    vn_encode_VkDevice(enc, &device);
    vn_encode_VkEvent(enc, &event);
}

static inline size_t vn_sizeof_vkGetEventStatus_reply(VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkGetEventStatus_EXT;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type);

    VkResult ret;
    cmd_size += vn_sizeof_VkResult(&ret);
    /* skip device */
    /* skip event */

    return cmd_size;
}

static inline VkResult vn_decode_vkGetEventStatus_reply(struct vn_cs_decoder *dec, VkDevice device, VkEvent event)
{
    VkCommandTypeEXT command_type;
    vn_decode_VkCommandTypeEXT(dec, &command_type);
    assert(command_type == VK_COMMAND_TYPE_vkGetEventStatus_EXT);

    VkResult ret;
    vn_decode_VkResult(dec, &ret);
    /* skip device */
    /* skip event */

    return ret;
}

static inline size_t vn_sizeof_vkSetEvent(VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkSetEvent_EXT;
    const VkFlags cmd_flags = 0;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type) + vn_sizeof_VkFlags(&cmd_flags);

    cmd_size += vn_sizeof_VkDevice(&device);
    cmd_size += vn_sizeof_VkEvent(&event);

    return cmd_size;
}

static inline void vn_encode_vkSetEvent(struct vn_cs_encoder *enc, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkSetEvent_EXT;

    vn_encode_VkCommandTypeEXT(enc, &cmd_type);
    vn_encode_VkFlags(enc, &cmd_flags);

    vn_encode_VkDevice(enc, &device);
    vn_encode_VkEvent(enc, &event);
}

static inline size_t vn_sizeof_vkSetEvent_reply(VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkSetEvent_EXT;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type);

    VkResult ret;
    cmd_size += vn_sizeof_VkResult(&ret);
    /* skip device */
    /* skip event */

    return cmd_size;
}

static inline VkResult vn_decode_vkSetEvent_reply(struct vn_cs_decoder *dec, VkDevice device, VkEvent event)
{
    VkCommandTypeEXT command_type;
    vn_decode_VkCommandTypeEXT(dec, &command_type);
    assert(command_type == VK_COMMAND_TYPE_vkSetEvent_EXT);

    VkResult ret;
    vn_decode_VkResult(dec, &ret);
    /* skip device */
    /* skip event */

    return ret;
}

static inline size_t vn_sizeof_vkResetEvent(VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkResetEvent_EXT;
    const VkFlags cmd_flags = 0;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type) + vn_sizeof_VkFlags(&cmd_flags);

    cmd_size += vn_sizeof_VkDevice(&device);
    cmd_size += vn_sizeof_VkEvent(&event);

    return cmd_size;
}

static inline void vn_encode_vkResetEvent(struct vn_cs_encoder *enc, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkResetEvent_EXT;

    vn_encode_VkCommandTypeEXT(enc, &cmd_type);
    vn_encode_VkFlags(enc, &cmd_flags);

    vn_encode_VkDevice(enc, &device);
    vn_encode_VkEvent(enc, &event);
}

static inline size_t vn_sizeof_vkResetEvent_reply(VkDevice device, VkEvent event)
{
    const VkCommandTypeEXT cmd_type = VK_COMMAND_TYPE_vkResetEvent_EXT;
    size_t cmd_size = vn_sizeof_VkCommandTypeEXT(&cmd_type);

    VkResult ret;
    cmd_size += vn_sizeof_VkResult(&ret);
    /* skip device */
    /* skip event */

    return cmd_size;
}

static inline VkResult vn_decode_vkResetEvent_reply(struct vn_cs_decoder *dec, VkDevice device, VkEvent event)
{
    VkCommandTypeEXT command_type;
    vn_decode_VkCommandTypeEXT(dec, &command_type);
    assert(command_type == VK_COMMAND_TYPE_vkResetEvent_EXT);

    VkResult ret;
    vn_decode_VkResult(dec, &ret);
    /* skip device */
    /* skip event */

    return ret;
}

static inline void vn_submit_vkCreateEvent(struct vn_instance *vn_instance, VkCommandFlagsEXT cmd_flags, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent, struct vn_instance_submit_command *submit)
{
    uint8_t local_cmd_data[VN_SUBMIT_LOCAL_CMD_SIZE];
    void *cmd_data = local_cmd_data;
    size_t cmd_size = vn_sizeof_vkCreateEvent(device, pCreateInfo, pAllocator, pEvent);
    if (cmd_size > sizeof(local_cmd_data)) {
        cmd_data = malloc(cmd_size);
        if (!cmd_data)
            cmd_size = 0;
    }
    const size_t reply_size = cmd_flags & VK_COMMAND_GENERATE_REPLY_BIT_EXT ? vn_sizeof_vkCreateEvent_reply(device, pCreateInfo, pAllocator, pEvent) : 0;

    struct vn_cs_encoder *enc = vn_instance_submit_command_init(vn_instance, submit, cmd_data, cmd_size, reply_size);
    if (cmd_size) {
        vn_encode_vkCreateEvent(enc, cmd_flags, device, pCreateInfo, pAllocator, pEvent);
        vn_instance_submit_command(vn_instance, submit);
        if (cmd_data != local_cmd_data)
            free(cmd_data);
    }
}

static inline void vn_submit_vkDestroyEvent(struct vn_instance *vn_instance, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator, struct vn_instance_submit_command *submit)
{
    uint8_t local_cmd_data[VN_SUBMIT_LOCAL_CMD_SIZE];
    void *cmd_data = local_cmd_data;
    size_t cmd_size = vn_sizeof_vkDestroyEvent(device, event, pAllocator);
    if (cmd_size > sizeof(local_cmd_data)) {
        cmd_data = malloc(cmd_size);
        if (!cmd_data)
            cmd_size = 0;
    }
    const size_t reply_size = cmd_flags & VK_COMMAND_GENERATE_REPLY_BIT_EXT ? vn_sizeof_vkDestroyEvent_reply(device, event, pAllocator) : 0;

    struct vn_cs_encoder *enc = vn_instance_submit_command_init(vn_instance, submit, cmd_data, cmd_size, reply_size);
    if (cmd_size) {
        vn_encode_vkDestroyEvent(enc, cmd_flags, device, event, pAllocator);
        vn_instance_submit_command(vn_instance, submit);
        if (cmd_data != local_cmd_data)
            free(cmd_data);
    }
}

static inline void vn_submit_vkGetEventStatus(struct vn_instance *vn_instance, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event, struct vn_instance_submit_command *submit)
{
    uint8_t local_cmd_data[VN_SUBMIT_LOCAL_CMD_SIZE];
    void *cmd_data = local_cmd_data;
    size_t cmd_size = vn_sizeof_vkGetEventStatus(device, event);
    if (cmd_size > sizeof(local_cmd_data)) {
        cmd_data = malloc(cmd_size);
        if (!cmd_data)
            cmd_size = 0;
    }
    const size_t reply_size = cmd_flags & VK_COMMAND_GENERATE_REPLY_BIT_EXT ? vn_sizeof_vkGetEventStatus_reply(device, event) : 0;

    struct vn_cs_encoder *enc = vn_instance_submit_command_init(vn_instance, submit, cmd_data, cmd_size, reply_size);
    if (cmd_size) {
        vn_encode_vkGetEventStatus(enc, cmd_flags, device, event);
        vn_instance_submit_command(vn_instance, submit);
        if (cmd_data != local_cmd_data)
            free(cmd_data);
    }
}

static inline void vn_submit_vkSetEvent(struct vn_instance *vn_instance, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event, struct vn_instance_submit_command *submit)
{
    uint8_t local_cmd_data[VN_SUBMIT_LOCAL_CMD_SIZE];
    void *cmd_data = local_cmd_data;
    size_t cmd_size = vn_sizeof_vkSetEvent(device, event);
    if (cmd_size > sizeof(local_cmd_data)) {
        cmd_data = malloc(cmd_size);
        if (!cmd_data)
            cmd_size = 0;
    }
    const size_t reply_size = cmd_flags & VK_COMMAND_GENERATE_REPLY_BIT_EXT ? vn_sizeof_vkSetEvent_reply(device, event) : 0;

    struct vn_cs_encoder *enc = vn_instance_submit_command_init(vn_instance, submit, cmd_data, cmd_size, reply_size);
    if (cmd_size) {
        vn_encode_vkSetEvent(enc, cmd_flags, device, event);
        vn_instance_submit_command(vn_instance, submit);
        if (cmd_data != local_cmd_data)
            free(cmd_data);
    }
}

static inline void vn_submit_vkResetEvent(struct vn_instance *vn_instance, VkCommandFlagsEXT cmd_flags, VkDevice device, VkEvent event, struct vn_instance_submit_command *submit)
{
    uint8_t local_cmd_data[VN_SUBMIT_LOCAL_CMD_SIZE];
    void *cmd_data = local_cmd_data;
    size_t cmd_size = vn_sizeof_vkResetEvent(device, event);
    if (cmd_size > sizeof(local_cmd_data)) {
        cmd_data = malloc(cmd_size);
        if (!cmd_data)
            cmd_size = 0;
    }
    const size_t reply_size = cmd_flags & VK_COMMAND_GENERATE_REPLY_BIT_EXT ? vn_sizeof_vkResetEvent_reply(device, event) : 0;

    struct vn_cs_encoder *enc = vn_instance_submit_command_init(vn_instance, submit, cmd_data, cmd_size, reply_size);
    if (cmd_size) {
        vn_encode_vkResetEvent(enc, cmd_flags, device, event);
        vn_instance_submit_command(vn_instance, submit);
        if (cmd_data != local_cmd_data)
            free(cmd_data);
    }
}

static inline VkResult vn_call_vkCreateEvent(struct vn_instance *vn_instance, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkCreateEvent(vn_instance, VK_COMMAND_GENERATE_REPLY_BIT_EXT, device, pCreateInfo, pAllocator, pEvent, &submit);
    struct vn_cs_decoder *dec = vn_instance_get_command_reply(vn_instance, &submit);
    if (dec) {
        const VkResult ret = vn_decode_vkCreateEvent_reply(dec, device, pCreateInfo, pAllocator, pEvent);
        vn_instance_free_command_reply(vn_instance, &submit);
        return ret;
    } else {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }
}

static inline void vn_async_vkCreateEvent(struct vn_instance *vn_instance, VkDevice device, const VkEventCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkEvent* pEvent)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkCreateEvent(vn_instance, 0, device, pCreateInfo, pAllocator, pEvent, &submit);
}

static inline void vn_call_vkDestroyEvent(struct vn_instance *vn_instance, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkDestroyEvent(vn_instance, VK_COMMAND_GENERATE_REPLY_BIT_EXT, device, event, pAllocator, &submit);
    struct vn_cs_decoder *dec = vn_instance_get_command_reply(vn_instance, &submit);
    if (dec) {
        vn_decode_vkDestroyEvent_reply(dec, device, event, pAllocator);
        vn_instance_free_command_reply(vn_instance, &submit);
    }
}

static inline void vn_async_vkDestroyEvent(struct vn_instance *vn_instance, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkDestroyEvent(vn_instance, 0, device, event, pAllocator, &submit);
}

static inline VkResult vn_call_vkGetEventStatus(struct vn_instance *vn_instance, VkDevice device, VkEvent event)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkGetEventStatus(vn_instance, VK_COMMAND_GENERATE_REPLY_BIT_EXT, device, event, &submit);
    struct vn_cs_decoder *dec = vn_instance_get_command_reply(vn_instance, &submit);
    if (dec) {
        const VkResult ret = vn_decode_vkGetEventStatus_reply(dec, device, event);
        vn_instance_free_command_reply(vn_instance, &submit);
        return ret;
    } else {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }
}

static inline void vn_async_vkGetEventStatus(struct vn_instance *vn_instance, VkDevice device, VkEvent event)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkGetEventStatus(vn_instance, 0, device, event, &submit);
}

static inline VkResult vn_call_vkSetEvent(struct vn_instance *vn_instance, VkDevice device, VkEvent event)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkSetEvent(vn_instance, VK_COMMAND_GENERATE_REPLY_BIT_EXT, device, event, &submit);
    struct vn_cs_decoder *dec = vn_instance_get_command_reply(vn_instance, &submit);
    if (dec) {
        const VkResult ret = vn_decode_vkSetEvent_reply(dec, device, event);
        vn_instance_free_command_reply(vn_instance, &submit);
        return ret;
    } else {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }
}

static inline void vn_async_vkSetEvent(struct vn_instance *vn_instance, VkDevice device, VkEvent event)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkSetEvent(vn_instance, 0, device, event, &submit);
}

static inline VkResult vn_call_vkResetEvent(struct vn_instance *vn_instance, VkDevice device, VkEvent event)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkResetEvent(vn_instance, VK_COMMAND_GENERATE_REPLY_BIT_EXT, device, event, &submit);
    struct vn_cs_decoder *dec = vn_instance_get_command_reply(vn_instance, &submit);
    if (dec) {
        const VkResult ret = vn_decode_vkResetEvent_reply(dec, device, event);
        vn_instance_free_command_reply(vn_instance, &submit);
        return ret;
    } else {
        return VK_ERROR_OUT_OF_HOST_MEMORY;
    }
}

static inline void vn_async_vkResetEvent(struct vn_instance *vn_instance, VkDevice device, VkEvent event)
{
    struct vn_instance_submit_command submit;
    vn_submit_vkResetEvent(vn_instance, 0, device, event, &submit);
}

#endif /* VN_PROTOCOL_DRIVER_EVENT_H */
