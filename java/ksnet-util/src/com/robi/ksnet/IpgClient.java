package com.robi.ksnet;

import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.net.InetSocketAddress;
import java.util.Arrays;

public class IpgClient {

    // �ɹ� ���� //
    private String ipgAddr; // ��� ���� �ּ�
    private int ipgPort;    // ��� ���� ��Ʈ
    
    private String traceId; // ���� ID
    private Logger logger;  // �ΰ�

    // �ɹ� �޼��� //
    public IpgClient(String ipgAddr, int ipgPort, String traceId, Logger logger) {
        this.ipgAddr = ipgAddr;
        this.ipgPort = ipgPort;
        this.traceId = traceId;
        this.logger = logger;
    }

    // IPG�� ���� �߽� �� ���� (���ŷ)
    public boolean sendAndRecvMsg(IpgMsg sendIpgMsg, IpgMsg recvIpgMsg, boolean localTest) {
        if (sendIpgMsg == null || recvIpgMsg == null) {
            log(Logger.ERROR, traceId, "One of sendIpg/recvIpgMsg is null! (sendIpgMsg:" + sendIpgMsg + ", recvIpgMsg:" + recvIpgMsg + ")");
            return false;
        }

        byte[] sendByteStream = sendIpgMsg.getMsgStream();

        // ���θ� ����ġ ��ȣȭ ������� ���̱�
        if (!localTest) {
            int originSendMsgLen = sendByteStream.length; // ���� ���� ����

            // {��ȣȭ���� ������ �������� ����(6) + ��ȣȭ����(1)}{��������(...)} (0:���ȣȭ)
            // ���������� �տ� 7����Ʈ�� ��� ��ġ�� ī��
            byte[] headedSendByteStream = new byte[originSendMsgLen + 7];
            System.arraycopy(sendByteStream, 0, headedSendByteStream, 7, originSendMsgLen);
            
            // {��ȣȭ ���� ���(���� + ��ȣȭ����)}
            // ���� 7����Ʈ�� ��ȣȭ ��� ���� (originSendMsgLen + 1 : ��ȣȭ���ε� ���̷� ����)
            byte[] headByteStream = String.format("%06d0", Math.min(originSendMsgLen + 1, 999999)).getBytes();
            System.arraycopy(headByteStream, 0, headedSendByteStream, 0, 7);
            
            sendByteStream = headedSendByteStream;
        }

        long sendMsgLen = sendByteStream.length;
        long recvMsgLen = recvIpgMsg.getMsgStream().length + (localTest ? 0 : 6);   // ���θ� ����ġ ������� 6 : ����������������(6)
        long remainSendLen = sendMsgLen;                                            // �߽��ؾ� �� ������ ���� ����
        long remainReadLen = recvMsgLen;                                            // �����ؾ� �� ������ ���� ����
        SocketChannel cliSoc = null;
        boolean rtResult = true;

        try {
            cliSoc = SocketChannel.open((SocketAddress)new InetSocketAddress(ipgAddr, ipgPort)); // Open and Connect

            ByteBuffer writeBuffer = ByteBuffer.wrap(sendByteStream);
            
            while (remainSendLen > 0) {
                // �⺻��� : ByteBuffer.remaining()ũ�⸸ŭ �� ������ ������ ���ŷ
                remainSendLen -= cliSoc.write(writeBuffer);
                Thread.sleep(1);
            }

            ByteBuffer readBuffer = ByteBuffer.allocate((int)remainReadLen);
            
            while (remainReadLen > 0) {
                // �⺻��� : ���Ź��� ������ ByteBuffer.remaining() ũ�⸸ŭ ���� �дµ��� ���ŷ
                long readLen = -1;

                if ((readLen = cliSoc.read(readBuffer)) > 0) {
                    if ((remainReadLen -= readLen) == 0) {
                        break;
                    }
                }
                else {
                    if (remainReadLen > 0) {
                        // ���Ź��� �����Ͱ� ����: ������ ���� �ݱ�
                        log(Logger.ERROR, traceId, "Error! The socket will be closed. (readLen:" + readLen +
                                                   ", remainReadLen:" + remainReadLen + ")");
                        
                        if (cliSoc != null && cliSoc.isConnected()) {
                            cliSoc.close();
                        }

                        rtResult = false;
                    }
                }
            }

            if (!localTest) { // ���θ� ����ġ ������� ����
                recvIpgMsg.setMsgStream(Arrays.copyOfRange(readBuffer.array(), 6, (int)recvMsgLen));
            }
            else {
                recvIpgMsg.setMsgStream(readBuffer.array());
            }
        }
        catch (Exception exception1) {
            log(Logger.ERROR, traceId, "Exception!", exception1);
            rtResult = false;
        }
        finally {
            try {
                if (cliSoc != null && cliSoc.isConnected()) {
                    cliSoc.close();
                }
            }
            catch (Exception exception2) {
                // ���� �ݱ� ���� ���ܴ� ���� ó���ϰ� ���з� ó������ �ʴ´�...
                log(Logger.WARNING, traceId, "Exception has occurred while closing the socket!", exception2);
            }

            return rtResult;
        }
    }

    private void log(int logType, String traceId, Object... logObjs) {
        if (logger != null) {
            logger.traceLog(logType, traceId, logObjs);
        }
    }
}
