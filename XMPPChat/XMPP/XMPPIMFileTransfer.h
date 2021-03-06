//
//  XMPPIMFileTransfer.h
//  XMPPChat
//
//  Created by 马远征 on 14-4-23.
//  Copyright (c) 2014年 马远征. All rights reserved.
//

#import "XMPPModule.h"
#import <XMPP.h>

#define KMaxBufferLen 4096
#define KMaxReadBytesLen 1024
#define KReadDataTimeOut -1
#define KWriteDataTimeOut -1


typedef enum XMPP_FILE_TYPE
{
    xmpp_FILE_UNKNOWN,
    XMPP_FILE_IMAGE,
    XMPP_FILE_VOICE,
    XMPP_FILE_VIDEO,
    XMPP_FILE_FILE,
    XMPP_FILE_OTHER,
    
}XMPP_FILE_TYPE;

@interface XmppFileModel : NSObject
@property (nonatomic, strong) NSString   *uuid;
@property (nonatomic, strong) NSString   *fileName;
@property (nonatomic, assign) UInt32 fileSize;
@property (nonatomic, strong) NSString   *mimetype;
@property (nonatomic, strong) NSString   *hashCode;
@property (nonatomic, strong) NSString   *filePath;
@property (nonatomic, strong) NSDate     *timeStamp;
@property (nonatomic, strong) XMPPJID    *JID;
@property (nonatomic, assign) BOOL       isOutGoing;
@property (nonatomic, assign) XMPP_FILE_TYPE fileType;
@end


#pragma mark -
#pragma mark xmppSocksConnect
///////////////////////////xep-065 xmpp socks协商///////////////////////////////////////////

@class xmppSocksConnect;

/**
 * XEP-065协议回调，如果成功，则返回一个激活的socket连接，否则返回失败
 */
@protocol xmppSKConnectDelegate <NSObject>
@required
- (void)xmppSocks:(xmppSocksConnect*)sender didSucceed:(GCDAsyncSocket*)socket;
- (void)xmppSocksDidFail:(xmppSocksConnect *)sender;

@end

@interface xmppSocksConnect : NSObject
@property (nonatomic, strong) GCDAsyncSocket *asyncSocket;
@property (nonatomic, OBJ_WEAK) id<xmppSKConnectDelegate> delegate;

// 检查是不是重复的文件传输请求
+ (BOOL)isNewStartSocksRequest:(XMPPIQ*)inIQ;

// 文件传输代理，你必须设置一个可用代理，以确保文件传输正常使用
+ (NSArray*)proxyCandidates;
+ (void)setProxyCandidates:(NSArray*)candidates;

- (id)initWithStream:(XMPPStream*)xmppStream toJID:(XMPPJID*)jid;
- (id)initWithStream:(XMPPStream *)xmppStream inIQRequest:(XMPPIQ *)inIQ;
- (void)startWithDelegate:(id)aDelegate delegateQueue:(dispatch_queue_t)aDelegateQueue;
- (void)abort;
@end


#pragma mark -
#pragma mark XMPPFileTransfer
///////////////////////////xep-096 xmpp文件传输///////////////////////////////////////////
@class XMPPFileTransfer;
@protocol xmppFileDelegate <NSObject>

- (void)xmppFileTrans:(XMPPFileTransfer*)sender willSendFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didSendFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didSuccessSendFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didFailSendFile:(XmppFileModel *)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender willReceiveFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didReceiveFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didSuccessReceiveFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didFailRecFile:(XmppFileModel *)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didRejectFile:(XmppFileModel*)file;
- (void)xmppFileTrans:(XMPPFileTransfer*)sender didUpdateUI:(NSUInteger)progressValue;
@end

@interface XMPPFileTransfer : NSObject
@property (nonatomic, strong) XMPPIQ *receiveIQ;
@property (nonatomic, OBJ_WEAK) id<xmppFileDelegate> delegate;
@property (nonatomic, strong) XmppFileModel *fileModel;

- (id)initWithStream:(XMPPStream*)xmppStream toJID:(XMPPJID*)jid;
- (id)initWithStream:(XMPPStream *)xmppStream inIQRequest:(XMPPIQ *)inIQ;
- (void)startWithDelegate:(id)aDelegate delegateQueue:(dispatch_queue_t)aDelegateQueue;
- (void)sendFileTransferRequest:(XMPPJID*)toJID
                       fileName:(NSString*)fileName
                       fileSize:(NSString*)fileSize
                       fileDesc:(NSString*)fileDesc
                       mimeType:(NSString*)mimeType
                           hash:(NSString*)hashCode
                           date:(NSString*)fileDate;
@end


#pragma mark -
#pragma mark XMPPIMFileManager
///////////////////////////xmpp文件传输管理///////////////////////////////////////////

@interface XMPPIMFileManager : XMPPModule
- (BOOL)sendImageWithData:(NSData*)imageData toJID:(XMPPJID*)jid;
@end
