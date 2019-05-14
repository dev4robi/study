package com.robi.ksnet;

import java.util.Arrays;
import java.util.Set;
import java.util.Iterator;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.InetAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

public class IpgRelayServer {
    
    // Ŭ���� ��� //
    public static int MSG_HEADER_SIZE = 4;          // ���� ��� ����
    public static int SERVER_RW_BUFFE_SIZE = 2048;  // ���� �б�&���� ���� ũ��
    public static int SERVER_PORT = 9595;           // ���� ��Ʈ
    public static long CLIENT_TIMEOUT = 10000;      // Ŭ���̾�Ʈ Ÿ�Ӿƿ�: 10��

    // Ŭ���� ���� ��� //
    private static final Logger _logger = Logger.getInstance();

    // �ɹ� ���� //
    private String svrName;                 // ���� �̸�
    private int svrStateCode;               // ���� �����ڵ� (0:�ʱ�ȭ, 1:�����Ϸ�, 2:�⵿��, 3:������)
    private long svrOpenTime;               // ���� �������� �ð�
    private Selector selector;              // ���� ���� ������
    private ServerSocketChannel svrSoc;     // ���� ���� ä��
    private List<Client> cliList;           // Ŭ���̾�Ʈ ����Ʈ

    // Ŭ���̾�Ʈ ���� Ŭ���� //
    static class Client {

        private SocketChannel cliSoc;   // Ŭ���̾�Ʈ ���� ä��
        private long connectTime;       // ������ ������ �ð�
        private long timeoutTime;       // Ÿ�Ӿƿ��� ������ �ð�
        private ByteBuffer recvBuffer;  // ���Ź��� ����Ʈ�� ������ ����

        public Client(SocketChannel cliSoc) {
            this.cliSoc = cliSoc;
            this.connectTime = System.currentTimeMillis();
            this.timeoutTime = this.connectTime + CLIENT_TIMEOUT;
            this.recvBuffer = ByteBuffer.allocate(SERVER_RW_BUFFE_SIZE);
        }

        // Ÿ�Ӿƿ� üũ
        public boolean isTimeouted(long curTime) {
            return (timeoutTime - curTime <= 0);
        }

        // Ÿ�Ӿƿ� ����
        public void renewTimeout(long curTime) {
            this.timeoutTime = curTime + CLIENT_TIMEOUT;
        }

        public SocketChannel getSoc() {
            return this.cliSoc;
        }

        public SelectionKey getSelKey(Selector svrSelector) {
            return this.cliSoc.keyFor(svrSelector);
        }

        public ByteBuffer getRecvBuf() {
            return this.recvBuffer;
        }

        public InetAddress getSocAddr() {
            InetAddress socAddr = null;

            try {
                socAddr = this.cliSoc.socket().getInetAddress();
            }
            catch (Exception exception) {
                _logger.log(Logger.ERROR, String.format("Ŭ���̾�Ʈ IP�ּ� ��ȯ ����! (���� �޽���: %s)", exception));
            }

            return socAddr;
        }

        public boolean addRecvByte(ByteBuffer recvBuffer) {
            int recvLen = recvBuffer.position();
            int remainLen = this.recvBuffer.remaining();

            if (remainLen - recvLen < 0) { // ���� ���� �������� �����͸� ����
                return false;
            }
            
            this.recvBuffer.put(recvBuffer.array(), 0, recvBuffer.capacity() - recvBuffer.remaining());

            return true;
        }
    }

    // �ɹ� �޼��� //
    public IpgRelayServer(String svrName) {
        this.svrName = svrName;
        this.svrStateCode = 0;
        this.svrOpenTime = System.currentTimeMillis();
    }

    // ���� ����
    public boolean openServer() {
        _logger.log(Logger.INFO, String.format("[%s] ���� ���� ����.", this.svrName));

        // ����� �˻�
        if (selector != null || svrSoc != null || cliList != null) {
            _logger.log(Logger.ERROR, String.format("[%s] ������ �̹� ������ ���� �ֽ��ϴ�. ���� �� �ٽ� �����Ͻʽÿ�.", this.svrName));
            return false;
        }

        boolean rtResult = false;

        // ���� ���� �� �ɹ� ���� �ʱ�ȭ
        try {        
            svrSoc = ServerSocketChannel.open();
            svrSoc.configureBlocking(false);
            svrSoc.socket().bind(new InetSocketAddress(SERVER_PORT));

            this.selector = Selector.open();
            svrSoc.register(this.selector, SelectionKey.OP_ACCEPT);

            cliList = new LinkedList<Client>();
            rtResult = true;
        }
        catch (Exception exception) {
            _logger.log(Logger.WARNING, String.format("[%s] ���� ������ ���� �߻�. (���� �޽���:%s)",
                                                     this.svrName, exception.getMessage()));
            rtResult = false;
        }
            
        // ��� ���
        if (rtResult) {
            _logger.log(Logger.INFO, String.format("[%s] ���� ���� ����.", this.svrName));
            this.svrStateCode = 1;
        }
        else {
            _logger.log(Logger.INFO, String.format("[%s] ���� ���� ����.", this.svrName));
        }

        return true;
    }

    // ���� ����
    public void closeServer() {
        _logger.log(Logger.INFO, String.format("[%s] ���� ���� ����.", this.svrName));

        this.svrStateCode = 3;

        // ��� Ŭ���̾�Ʈ ���� ����
        Set<SelectionKey> selKeySet = this.selector.keys();
        Iterator<SelectionKey> selKeyIter = selKeySet.iterator();
        SelectionKey selKey = null;

        while (selKeyIter.hasNext()) {
            selKey = selKeyIter.next();

            if (selKey.channel() instanceof SocketChannel) {
                closeClient(selKey);
            }
        }

        // ���� �ڿ� ����
        try {
            this.svrSoc.close();
            this.selector.close();
        }
        catch (Exception exception) {
            _logger.log(Logger.WARNING, String.format("[%s] ���� ������ ���� �߻�. �ڿ� ���� ����! (���� �޽���: %s)",
                                                  this.svrName, exception.getMessage()));
        }

        this.svrStateCode = 0;
        _logger.log(Logger.INFO, String.format("[%s] ���� ���� �Ϸ�.", this.svrName));
    }

    // ���� ����
    public boolean runServer() {
        _logger.log(Logger.INFO, String.format("[%s] ���� ���� ����.", this.svrName));

        if (this.svrStateCode != 1) {
            _logger.log(Logger.ERROR, String.format("[%s] ������ ������ �� ����. ������ '�����Ϸ�(1)' ���°� �ƴմϴ�.", this.svrName));
            return false;
        }
        
        this.svrStateCode = 2;
        long lastIdleTime = 0;              // ���������� Idle�� ����� �ð�
        long nextTimeoutUpdateTime = 0;     // ���� Timeout ������Ʈ �ð�
        long nextLifeLoggingTime = 0;       // ���� ���� �α� ����� �ð�
        
        while (true) {
            if (this.svrStateCode != 2) {
                break;
            }

            try {
                // ������ ����
                if (this.selector.select(1) == 0) {
                    // ó���� ������ ���� ��� Idle���� ����
                    lastIdleTime = System.currentTimeMillis();

                    // Ŭ���̾�Ʈ Ÿ�Ӿƿ� ���� (1�ʿ� �� ��)
                    if (nextTimeoutUpdateTime <= lastIdleTime) {
                        updateClientsTimeout();
                        nextTimeoutUpdateTime = System.currentTimeMillis() + 1000; // Ŭ���̾�Ʈ Ÿ�Ӿƿ� ó���� ���� �ɸ� �� �����Ƿ� ���� �ð��� �Ź� �ٽ� ����
                    }

                    // ���� �α� (60�ʿ� �� ��)
                    if (nextLifeLoggingTime <= lastIdleTime) {
                        _logger.log(Logger.INFO, String.format("[%s] ���� ������... (���� Ŭ���̾�Ʈ:%d / ��� �ð�:%dms)",
                                                              this.svrName, this.cliList.size(), lastIdleTime - this.svrOpenTime));
                        nextLifeLoggingTime = lastIdleTime + 60000;
                    }

                    continue;
                }

                // �����ͷ� �޾ƿ� SelectorKey�� ����
                Set<SelectionKey> selKeySet = this.selector.selectedKeys();
                Iterator<SelectionKey> selKeyIter = selKeySet.iterator();

                while (selKeyIter.hasNext()) {
                    SelectionKey selKey = selKeyIter.next();

                    selKeyIter.remove();
                    
                    if (!selKey.isValid()) { // Ű ��ȿ�� �˻�
                        _logger.log(Logger.WARNING, String.format("[%s] InValid SelectionKey! (%s)", this.svrName, selKey.toString()));
                        continue;
                    }
                    
                    if (selKey.isAcceptable()) { // Ŭ���̾�Ʈ ����
                        acceptClient(selKey);
                        continue;
                    }
                    else if (selKey.isReadable()) { // Ŭ���̾�Ʈ �����ͼ��� �� ������Ʈ
                        updateClient(readFromClient(selKey));
                        closeClient(selKey); // �������� ������� ������Ʈ �� ���� ���� (1ȸ�� ����)
                        continue;
                    }
                    else if (selKey.isWritable()) {
                        // ...
                        continue;
                    }
                }
            }
            catch (Exception exception) {
                _logger.log(Logger.ERROR, String.format("[%s] ���� ������ ���� �߻�. (���� �޽���: %s)",
                                                       this.svrName, exception.getMessage()));
            }
        }

        _logger.log(Logger.INFO, String.format("[%s] ���� ���� ����.", this.svrName));

        return true;
    }

    // Ŭ���̾�Ʈ ����Ʈ���� Ŭ���̾�Ʈ ��ȯ
    public Client getClientFromList(SelectionKey selKey) {
        Client cli = null;
        int szCliList = this.cliList.size();
        SocketChannel cliSoc = (SocketChannel)selKey.channel();

        for (int i = 0; i < szCliList; ++i) {
            cli = this.cliList.get(i);

            if (cliSoc == cli.getSoc()) {
                return cli;
            }
        }

        return null;
    }

    // Ŭ���̾�Ʈ ����
    public void acceptClient(SelectionKey selKey) {
        try {
            SocketChannel cliSoc = ((ServerSocketChannel)selKey.channel()).accept();
            cliSoc.configureBlocking(false);
            cliSoc.register(this.selector, SelectionKey.OP_READ);

            Client client = new Client(cliSoc);
            this.cliList.add(client);

            InetAddress cliAddr = client.getSocAddr();
            _logger.log(Logger.INFO, String.format("[%s] Ŭ���̾�Ʈ '%s' Accept����. (���� Ŭ���̾�Ʈ: %d)",
                                                  this.svrName, cliAddr.toString(), cliList.size()));
        }
        catch (Exception exception) {
            _logger.log(Logger.ERROR, String.format("[%s] Ŭ���̾�Ʈ Accept ����! (���� �޽���:%s)",
                                                   this.svrName, exception.getMessage()));
        }
    }

    // Ŭ���̾�Ʈ ���� ����
    public void closeClient(SelectionKey selKey) {
        SocketChannel cliSoc = null;
        InetAddress cliAddr = null;

        try {
            Client cli = this.getClientFromList(selKey);

            cliSoc = cli.getSoc();
            cliAddr = cli.getSocAddr();

            // Ŭ���̾�Ʈ ����Ʈ���� ����
            if (cli != null) {
                this.cliList.remove(cli);
            }
            else {
                _logger.log(Logger.WARNING, String.format("[%s] Close�� Ŭ���̾�Ʈ '%s'�� cliList�� �������� ����! (����� Ŭ���̾�Ʈ: %d)",
                                                         this.svrName, cliAddr.toString(), cliList.size()));
            }

            // ������ ����
            selKey.cancel();

            // ���� �ݱ�
            cliSoc.close();
            _logger.log(Logger.INFO, String.format("[%s] Ŭ���̾�Ʈ '%s' ���� �ݱ� ����. (����� Ŭ���̾�Ʈ: %d)",
                                                  this.svrName, cliAddr.toString(), cliList.size()));
        }
        catch (Exception exception) {
            _logger.log(Logger.ERROR, String.format("[%s] Ŭ���̾�Ʈ '%s' ���� �ݱ� ����! (����� Ŭ���̾�Ʈ: %d)",
                                                   this.svrName, cliAddr.toString(), cliList.size()));
        }
    }

    // Ŭ���̾�Ʈ ��û �б�
    public Client readFromClient(SelectionKey selKey) {
        boolean closeCliFlag = false;
        InetAddress cliAddr = null;
        Client cli = null;

        try {
            SocketChannel cliSoc = (SocketChannel)selKey.channel();
            ByteBuffer readBuffer = ByteBuffer.allocate(SERVER_RW_BUFFE_SIZE);
            int readLen = 0;

            cli = this.getClientFromList(selKey);

            if (cli == null) {
                // Ŭ���̾�Ʈ ����Ʈ���� ã�� �� ������ ������ ����
                throw new Exception("Ŭ���̾�Ʈ�� Ŭ���̾�Ʈ ����Ʈ���� ã�� �� ����!");
            }

            cliAddr = cli.getSocAddr();

            if ((readLen = cliSoc.read(readBuffer)) > 0) { // ���Ͽ��� �����͸� ���������� ����                
                // Ŭ���̾�Ʈ�� ���Ź��ۿ� �߰�
                if (!cli.addRecvByte(readBuffer)) {
                    _logger.log(Logger.WARNING, String.format("[%s] ������ �ս� �߻�! Ŭ���̾�Ʈ '%s'�� ������ [%s]�� �� Ŭ���̾�Ʈ ���� ���۰� ������.",
                                                             this.svrName, cliAddr.toString(), new String(readBuffer.array())));
                }
            }
            else { // Ŭ���̾�Ʈ ������ �����ų� ����
                closeCliFlag = true;
            }
        
            _logger.log(Logger.INFO, String.format("[%s] Recv from client(%s) Msg:[%s]",
                                                  this.svrName, cliAddr.toString(), new String(readBuffer.array())));
        }
        catch (Exception exception) {
            exception.printStackTrace();
            closeCliFlag = true;
            _logger.log(Logger.ERROR, String.format("[%s] Recv from client(%s) ����!. (���� �޽���:%s)",
                                                   this.svrName, cliAddr.toString(), exception.getMessage()));
        }
        finally {
            if (closeCliFlag) {
                closeClient(selKey);
            }
        }

        return cli;
    }

    // Ŭ���̾�Ʈ ��û ����
    public boolean updateClient(Client cli) {
        InetAddress cliAddr = null;

        // ����� �ٵ� �и��Ͽ� ���� ����
        ByteBuffer cliRecvBuf = cli.getRecvBuf();
        byte[] headBytes = new byte[MSG_HEADER_SIZE];
        byte[] bodyBytes = null;
        byte[] msgBytes = null; // head + body Bytes

        try {
            cliAddr = cli.getSocAddr();
            cliRecvBuf.rewind();
            cliRecvBuf.get(headBytes);
            
            int bodyLen = Integer.valueOf(new String(headBytes));
            bodyBytes = new byte[bodyLen];

            cliRecvBuf.get(bodyBytes);
            cliRecvBuf.compact();

            msgBytes = new byte[headBytes.length + bodyBytes.length];
            System.arraycopy(headBytes, 0, msgBytes, 0, headBytes.length);
            System.arraycopy(bodyBytes, 0, msgBytes, headBytes.length, bodyBytes.length);
        }
        catch (Exception exception) {
            exception.printStackTrace();
            _logger.log(Logger.ERROR, String.format("[%s] Ŭ���̾�Ʈ(%s) ���� ���� ����! (���� �޽���:%s)",
                                                   this.svrName, cliAddr.toString(), exception.getMessage()));
            return false;
        }
        
        // ������ ���� �з� �� ���� ����
        byte tb1 = msgBytes[72], tb2 = msgBytes[73];
        IpgMsg cliRecvMsg = null;
        IpgMsg cliSendMsg = null;
        boolean serviceResult = false;

        if (tb1 == 'N' && tb2 == 'I') { // ����뺸 ����
            cliRecvMsg = new IpgMsg(msgBytes, IpgMsg.ReqPayNotifyAttr, null, null);
            cliSendMsg = new IpgMsg(null,     IpgMsg.RpyPayNotifyAttr, null, null);
            serviceResult = IpgService.payNotifyService(cli, cliRecvMsg, cliSendMsg);
        }
        else if (tb1 == 'M' && tb2 == 'I') { // ������ ������ȸ ����
            cliRecvMsg = new IpgMsg(msgBytes, IpgMsg.ReqShopInfoMsgAttr, null, null);
            cliSendMsg = new IpgMsg(null,     IpgMsg.RpyShopInfoMsgAttr, null, null);
            serviceResult = IpgService.shopInfoService(cli, cliRecvMsg, cliSendMsg);
        }
        else if (false) {
            // ... ���ο� ���� �߰�
        }
        else { // ���� ����
            _logger.log(Logger.ERROR, String.format("[%s] Ŭ���̾�Ʈ(%s) ���� ����! (�� �� ���� ���� �з�: '%c%c')",
                                                   this.svrName, cliAddr.toString(), tb1, tb2));
            return false;
        }

        // Ŭ���̾�Ʈ���� ��� ����
        if (cliRecvMsg != null) {
            this.writeToClient(cli.getSoc(), cliSendMsg.getMsgStream());
        }

        return serviceResult;
    }

    // Ŭ���̾�Ʈ ��û ����
    public void writeToClient(SocketChannel cliSoc, byte[] writeBytes) {
        InetAddress cliAddr = null;

        try {
            cliAddr = cliSoc.socket().getInetAddress();

            ByteBuffer writeBuf = ByteBuffer.wrap(writeBytes);
            int writeRemainLen = writeBytes.length;

            while (writeRemainLen > 0) {
                writeRemainLen -= cliSoc.write(writeBuf);
            }

            _logger.log(Logger.INFO, String.format("[%s] Send to client(%s). MSG:[%s]",
                                                  this.svrName, cliAddr.toString(), new String(writeBytes)));
        }
        catch (Exception exception) {
            exception.printStackTrace();
            _logger.log(Logger.ERROR, String.format("[%s] Send to client(%s) ����! (���� �޽���:%s)",
                                                   this.svrName, cliAddr.toString(), exception.getMessage()));
        }
    }

    // Ŭ���̾�Ʈ Ÿ�Ӿƿ� Ȯ��
    public void updateClientsTimeout() {
        long curTime = System.currentTimeMillis();

        for (Client cli : cliList) {
            if (cli.isTimeouted(curTime)) {
                closeClient(cli.getSelKey(this.selector));
            }
        }
    }

    public static void main(String[] args) {
        // ���� ����
        IpgRelayServer svr = new IpgRelayServer("TestIpgSvr");

        if (!svr.openServer()) {
            _logger.log(Logger.ERROR, "���� Open ����. �ٽ� �õ��Ͻʽÿ�.");
            return;
        }

        // ���� ����
        if (!svr.runServer()) {
            _logger.log(Logger.ERROR, "���� Run ����. �ٽ� �õ��Ͻʽÿ�.");
            return;
        }

        // ���� �ݰ� ���α׷� ����
        svr.closeServer();
        _logger.log(Logger.INFO, "���� ���μ����� �����մϴ�.");
    }
}