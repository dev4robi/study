package com.ksnet.main.server;

import java.util.Arrays;
import java.util.Set;
import java.util.Iterator;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

import com.ksnet.util.*;

public class RelayServerMain {

	public static HashMap<String, String> envHash;	// ȯ���ؽ�
	public static RelayServer relayServer;			// �߰����� ��ü

	// ����
	public static void main(String[] args) {
		try {
			init(args);
			run();
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_CRIT, e);
			Logger.logln(Logger.LogType.LT_CRIT, "���� �߻�. 2���� ����˴ϴ�.");
		}
		finally {
			try { Thread.sleep(120000); } catch (Exception e) {}
		}
	}
		
	// ���α׷� �ʱ�ȭ
	private static void init(String[] args) {		
		// ȯ�� �ؽ� �ʱ�ȭ
		envHash = new HashMap<String, String>();
		envHash.put("MY_PORT", args[0]);
		envHash.put("DEST_IP", args[1]);
		envHash.put("DEST_PORT", args[2]);
		envHash.put("DEST_SOCKET_CNT", args[3]);
		envHash.put("LOG_LEVEL", args[4]);
		
		// �ΰ� �ʱ�ȭ
		String logLevel = new String(getEnv("LOG_LEVEL"));
		Logger.setVisibleByLogLevel(logLevel);
		
		// ���� ����
		relayServer = new RelayServer();
	}
	
	// ���� ����
	private static void run() {
		if (relayServer != null && relayServer.checkRunnable()) {
			relayServer.run();
		}
	}
	
	// Ű�� ȯ�氪 ��������
	public static String getEnv(String key) {
		String envVal = null;
		
		try {
			envVal = envHash.get(key);
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
		finally {
			return envVal;
		}
	}
}

class RelayServer {
	
	private Selector selector;	// �߰����� Selector
	
	private ServerSocketChannel serverSocketChannel;		// �߰����� ServerSocketChannel
	private List<SocketChannel> clientList;					// �߰������� ����� Ŭ���̾�Ʈ SocketChannel
	private long clientTimeout;								// �߰������� Ŭ���̾�Ʈ�� Ÿ�Ӿƿ� �ð�
	private Map<SelectionKey, Long> clientLastReadTimeMap;	// �߰������� ����� Ŭ���̾�Ʈ�κ����� ������ ���Žð��� ����
	
	private String bankSvrIp;				// ���༭�� Ip
	private int bankSvrPort;				// ���༭�� Port
	private List<SocketChannel> serverList;	// �߰������� ����� ���༭�� SocketChannel
	private int curChannelIndex;			// ���༭���� �����ϱ����� ���� ä�� �ε���;
	
	private Map<Integer, SocketChannel> cliSvrMapping;	// Ŭ���̾�Ʈ�� ���༭���� �����ϴ� ��
	
	private TransceiveLogger transceiveLogger;	// ä�� ���Ź߽ſ� ���Ǵ� �ΰ�
	
	// ������
	public RelayServer() {
		initServer();
	}
	
	// ���� �������ɿ��� ����
	public boolean checkRunnable() {
		if (selector == null || !selector.isOpen()) return false;
		if (serverSocketChannel == null || !serverSocketChannel.isOpen()) return false;
		if (clientList == null) return false;
		if (serverList == null && serverList.size() == 0) return false;
		
		return true;
	}
	
	// ���� �ʱ�ȭ
	public void initServer() {
		Logger.logln(Logger.LogType.LT_DEBUG, "���� �ʱ�ȭ ����.");
		
		try {
			selector = Selector.open();
			serverSocketChannel = ServerSocketChannel.open();
			serverSocketChannel.configureBlocking(false);
			serverSocketChannel.bind(new InetSocketAddress(Integer.parseInt(RelayServerMain.getEnv("MY_PORT"))));
			serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
			
			clientList = new LinkedList<SocketChannel>();
			serverList = new LinkedList<SocketChannel>();
			
			clientTimeout = 60000; // Ŭ���̾�Ʈ �ִ� 1�а� ���� ����
			clientLastReadTimeMap = new HashMap<SelectionKey, Long>();
			
			bankSvrIp = RelayServerMain.getEnv("DEST_IP");
			bankSvrPort = Integer.parseInt(RelayServerMain.getEnv("DEST_PORT"));
			
			connectSocketChannel(bankSvrIp, bankSvrPort);
			
			curChannelIndex = 0;
			cliSvrMapping = new HashMap<Integer, SocketChannel>();
			
			transceiveLogger = new TransceiveLogger("res/output/RelayServer.out");
			
			Logger.logln(Logger.LogType.LT_DEBUG, "���� �ʱ�ȭ �Ϸ�.");
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_DEBUG, "���� �ʱ�ȭ ����.");
			Logger.logln(Logger.LogType.LT_ERR, e);
			closeServer();
		}
	}
	
	// ���� �ݱ�
	public void closeServer() {
		Logger.logln(Logger.LogType.LT_INFO, "���� �ݱ� ����.");
		
		try {
			if (clientList != null && clientList.size() > 0) {
				disconnectClientAll();
			}
			
			
			if (serverSocketChannel != null && serverSocketChannel.isOpen()) {
				serverSocketChannel.close();
				serverSocketChannel = null;
			}
			
			if (selector != null && selector.isOpen()) {
				selector.close();
				selector = null;
			}
			
			if (transceiveLogger != null) {
				transceiveLogger.writeToFile();
			}
			
			Logger.logln(Logger.LogType.LT_INFO, "���� �ݱ� �Ϸ�.");
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_INFO, "���� �ݱ� ����.");
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
	}
	
	// ���� �ð�����(Timeout) �ۼ����� ���� SocketChannel �ݰ�, Ŭ��-���༭�� ���� �ʱ�ȭ ����
	public void closeTimeoutedSocketChannelAll() {
		Set<SelectionKey> selectorKeys = selector.keys();
		
		for (SelectionKey key : selectorKeys) {
			try {
				Long lastReadTime = null;
				
				if ((lastReadTime = clientLastReadTimeMap.get(key)) == null) {
					continue; // Ŭ���̾�Ʈ ����ä���� �ƴ� ��� ���� Ű �˻�
				}
				
				long curTime = System.currentTimeMillis();
				long elapsedTime = curTime - lastReadTime;
				
				if (elapsedTime > clientTimeout) {
					Logger.logln(Logger.LogType.LT_DEBUG, ("Ÿ�Ӿƿ����� Ŭ���̾�Ʈ(" + getSocketChannelAddr((SocketChannel)key.channel()) + ")���� ������ �����մϴ�. (elapsedTime: " + elapsedTime + ", ���� ����: " + (clientList.size() - 1) + ")"));
					closeSocketChannel(key);
				}
			}
			catch (Exception e) {
				Logger.logln(Logger.LogType.LT_ERR, e);
			}
		}
		
		if (clientList.size() == 0 && cliSvrMapping.size() > 0) {
			cliSvrMapping.clear();
			Logger.logln(Logger.LogType.LT_INFO, ("����� Ŭ���̾�Ʈ�� �������� �ʾ� Ŭ��-���༭�� ������ �ʱ�ȭ�մϴ�."));
		}
	}
	
	// ���� ����
	public void run() {
		long stateLogTermMs = 5000;
		long lastStateLogTime = 0;
		
		Logger.logln(Logger.LogType.LT_DEBUG, "���� ���� ����.");

		while (true) {
			try {
				if (selector == null) break;
				
				// �ּ� 1ms���� select����� ���������� ���ŷ (Ŭ���̾�Ʈ�� ������ ���� ����Ǹ� if������ �������� ���� �� ������ ����)
				if (selector.select(1) == 0) {
					closeTimeoutedSocketChannelAll();
					
					long curTime = System.currentTimeMillis();
					
					if (curTime - lastStateLogTime > stateLogTermMs) {
						Logger.logln(Logger.LogType.LT_INFO, "[" + curTime + "] ����� Ŭ���̾�Ʈ: " + clientList.size() + ", ����� ����: " + serverList.size() + ", Ŭ��/���� ����: " + cliSvrMapping.size() + ", Ŭ��/Ÿ�Ӿƿ� ����: " + clientLastReadTimeMap.size());
						lastStateLogTime = System.currentTimeMillis();
					}
					
					transceiveLogger.writeToFile();
					continue;
				}
				
				// selector�� ���� key�鿡 �ش��ϴ� ��� ����
				Set<SelectionKey> selectedKeySet = selector.selectedKeys();
				Iterator<SelectionKey> iter = selectedKeySet.iterator();
				
				while (iter.hasNext()) {
					SelectionKey selectionKey = iter.next();
					
					iter.remove();
					
					if (!selectionKey.isValid()) {
						continue;
					}
					else if (selectionKey.isAcceptable()) {
						clientAccept(selectionKey);
					}
					else if (selectionKey.isReadable()) {
						read(selectionKey);
					}
					else if (selectionKey.isWritable()) {
						// write(selectionKey);
					}
				}
			}
			catch (Exception e) {
				Logger.logln(Logger.LogType.LT_ERR, "���� ������ ���� �߻�.");
				Logger.logln(Logger.LogType.LT_ERR, e);
			}
		}
		
		Logger.logln(Logger.LogType.LT_INFO, "���� ���� ����.");
	}
	
	// Ŭ���̾�Ʈ ���� ����
	private void clientAccept(SelectionKey selectionKey) {
		try {			
			ServerSocketChannel serverSocketChannel = (ServerSocketChannel)selectionKey.channel();
			SocketChannel socketChannel = serverSocketChannel.accept();
			
			socketChannel.configureBlocking(false);
			socketChannel.register(selector, SelectionKey.OP_READ);
			clientList.add(socketChannel);
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
		}
	}
	
	// ��� Ŭ���̾�Ʈ ���� ����
	private void disconnectClientAll() {
		Logger.logln(Logger.LogType.LT_DEBUG, "��� Ŭ���̾�Ʈ(" + clientList.size() + ")�� ���� ���Ḧ �����մϴ�.");
		
		Set<SelectionKey> keySet = selector.keys();
		Iterator<SelectionKey> iter = keySet.iterator();
		SelectionKey key = null;
		SocketChannel socket = null;
		
		while (iter.hasNext()) {
			key = iter.next();
			
			if (key.channel() instanceof SocketChannel) { // ServerSocketChannel ����
				socket = (SocketChannel)key.channel();
				
				if (serverList.indexOf(socket) == -1) { // Ŭ���̾�Ʈ SocketChannel�� ��� true
					closeSocketChannel(key);
				}
			}
		}
		
		Logger.logln(Logger.LogType.LT_DEBUG, "��� Ŭ���̾�Ʈ�� ���� ���Ḧ �Ϸ��߽��ϴ�.");
	}
	
	// �߰������� ���� ��� �Է� ó��
	private void read(SelectionKey selectionKey) {
		SocketChannel socketChannel = (SocketChannel)selectionKey.channel();
		int svrSocketIndex = serverList.indexOf(socketChannel);
		
		if (svrSocketIndex == -1) {
			read_client(selectionKey);
		}
		else {
			read_bankServer(selectionKey);
		}
	}
	
	// Ŭ���̾�Ʈ �Է� ó��
	private void read_client(SelectionKey selectionKey) {
		SocketChannel clientSocket = (SocketChannel)selectionKey.channel();
		byte[] readByte = null;
		
		if ((readByte = readSocketChannel(clientSocket, 306)) == null) {
			Logger.logln(Logger.LogType.LT_DEBUG, "Ŭ���̾�Ʈ(" + getSocketChannelAddr(clientSocket) + ")�� ������ �����߽��ϴ�.");
			closeSocketChannel(selectionKey);
			return;
		}
		
		// Ŭ���̾�Ʈ�κ��� ������ �ð� ����
		clientLastReadTimeMap.put(selectionKey, System.currentTimeMillis());
		
		// ���� ��ȣ�� Ű�� Ŭ���̾�Ʈ SocketChannel ����
		Integer hashKey = Integer.parseInt(new String(Arrays.copyOfRange(readByte, 32, 38)));
		cliSvrMapping.put(hashKey, clientSocket);
		
		// ���� ������ ����
		Logger.logln(Logger.LogType.LT_DEBUG, "hashKey: " + hashKey + ", svrSocIdx: " + curChannelIndex);
		
		if (serverList.size() > 0) {
			SocketChannel bankServerSocket = serverList.get(updateCurChannelIndex());
			writeSocketChannel(bankServerSocket, readByte);
		}
		else {
			connectSocketChannel(bankSvrIp, bankSvrPort);
		}
	}
	
	// ���༭�� �Է� ó��
	private void read_bankServer(SelectionKey selectionKey) {
		SocketChannel bankServerSocket = (SocketChannel)selectionKey.channel();
		byte[] readByte = null;
		
		if ((readByte = readSocketChannel(bankServerSocket, 306)) == null) {
			Logger.logln(Logger.LogType.LT_INFO, "���༭��(" + getSocketChannelAddr(bankServerSocket) + ")�� ������ ������׽��ϴ�.");
			
			if (clientList.size() > 0) {
				disconnectClientAll();
				reconnectSocketChannel(selectionKey);
			}
			else {
				closeSocketChannel(selectionKey);
			}
			
			return;
		}
		
		// ���� ��ȣ�� Ű�� Ŭ���̾�Ʈ SocketChannel �ҷ���
		Integer hashKey = Integer.parseInt(new String(Arrays.copyOfRange(readByte, 32, 38)));
		SocketChannel clientSocket = cliSvrMapping.remove(hashKey);
		
		if (clientSocket == null) {
			Logger.logln(Logger.LogType.LT_ERR, "clientSocket == null (hashKey: " + hashKey + ")");
			return;
		}
		
		// Ŭ���̾�Ʈ���� ����
		writeSocketChannel(clientSocket, readByte);
	}
	
	// ����ä�� ����
	private void connectSocketChannel(String destIp, int destPort) {
		int socketIndex = -1;
		
		try {
			String bankServerIp = RelayServerMain.getEnv("DEST_IP");
			int bankServerPort = Integer.parseInt(RelayServerMain.getEnv("DEST_PORT"));
			
			if (destIp.equals(bankServerIp) && destPort == bankServerPort) { // ���༭�� ����
				int socketCnt = Integer.parseInt(RelayServerMain.getEnv("DEST_SOCKET_CNT"));
				
				for (int i = serverList.size(); i < socketCnt; ++i) {
					socketIndex = serverList.size();
					
					Logger.logln(Logger.LogType.LT_INFO, ("�߰����� -> ���༭��(" + destIp + ":" + destPort + ") ���� �õ�. (" + socketIndex + ")"));
					
					SocketChannel toSvrChannel = SocketChannel.open();
					
					toSvrChannel.connect(new InetSocketAddress(destIp, destPort));
					toSvrChannel.configureBlocking(false);
					toSvrChannel.register(selector, SelectionKey.OP_READ);
					serverList.add(toSvrChannel);
					curChannelIndex = serverList.size() - 1;
					
					Logger.logln(Logger.LogType.LT_INFO, ("�߰����� -> ���༭��(" + destIp + ":" + destPort + ") ���� �Ϸ�. (" + socketIndex + ")"));
				}
			}
			else {
				Logger.logln(Logger.LogType.LT_INFO, ("SocketChannel ���� �ź�. ���ǵ��� ���� Ip:Port(" + destIp + ":" + destPort + ")"));
			}
		}
		catch (Exception e) {
			Logger.logln(Logger.LogType.LT_ERR, e);
			Logger.logln(Logger.LogType.LT_INFO, ("�߰����� -> ���༭��(" + destIp + ":" + destPort + ") ���� ����. (" + socketIndex + ")"));
		}
	}
	
	// ����ä�� �ݱ�
	private void closeSocketChannel(SelectionKey selectionKey) {
		try {
			SocketChannel socketChannel = (SocketChannel)selectionKey.channel();
			
			if (selectionKey != null) selectionKey.cancel();
			
			if (socketChannel != null && socketChannel.isOpen()) {
				InetSocketAddress remoteAddr = getSocketChannelAddr(socketChannel);
				
				socketChannel.close();
				
				if (clientList.remove(socketChannel)) {
					clientLastReadTimeMap.remove(selectionKey);
					Logger.logln(Logger.LogType.LT_DEBUG, "Ŭ���̾�Ʈ(" + remoteAddr + ")���� ���� �ϳ��� ����Ǿ����ϴ�. (���� ����: " + clientList.size() + ")");
				}
				else if (serverList.remove(socketChannel)) {
					Logger.logln(Logger.LogType.LT_WARN, "���༭��(" + remoteAddr + ")���� ���� �ϳ��� ����Ǿ����ϴ�. (���� ����: " + serverList.size() + ")");
				}
				else {
					Logger.logln(Logger.LogType.LT_WARN, "�� �� ���� ����(" + remoteAddr + ") �ϳ��� ����Ǿ����ϴ�.");
				}
			}
		}
		catch (IOException ioe) {
			Logger.logln(Logger.LogType.LT_ERR, ioe);
		}
	}
	
	// ���ϼ��� �翬��
	private void reconnectSocketChannel(SelectionKey selectionKey) {
		closeSocketChannel(selectionKey);
		connectSocketChannel(bankSvrIp, bankSvrPort);
	}
	
	// SocketChannel�� ������ read�Ͽ� ��ȯ�ϴ� �Լ�
	private byte[] readSocketChannel(SocketChannel socketChannel, int bufferLength) {
		ByteBuffer readBuffer = ByteBuffer.allocate(bufferLength);
		byte[] readByte = null;
		int readLength = -1;
		
		try {
			if ((readLength = socketChannel.read(readBuffer)) == -1) {
				return null;
			}
		}
		catch (IOException ioe) {
			Logger.logln(Logger.LogType.LT_ERR, ioe);
			return null;
		}
		
		readByte = new byte[readLength];
		System.arraycopy(readBuffer.array(), 0, readByte, 0, readByte.length);
		
		String recvAddr = getSocketChannelAddr(socketChannel).toString();
		Logger.logln(Logger.LogType.LT_DEBUG, ("[RecvFrom:" + recvAddr + "](" + new String(readByte) + ")"));
		transceiveLogger.log(String.format("[%d]rcv(%s:%04d)=(", System.currentTimeMillis(), recvAddr, readByte.length), new String(readByte), ")");
		
		return readByte;
	}
	
	// SocketChannel�� �������� write�ϴ� �Լ�
	private boolean writeSocketChannel(SocketChannel socketChannel, byte[] writeByte) {
		ByteBuffer writeBuffer = ByteBuffer.allocate(writeByte.length);
		
		writeBuffer.put(writeByte);
		writeBuffer.clear();
		
		try {
			socketChannel.write(writeBuffer);
		}
		catch (IOException ioe) {
			Logger.logln(Logger.LogType.LT_ERR, ioe);
			return false;
		}
		
		String recvAddr = getSocketChannelAddr(socketChannel).toString();
		Logger.logln(Logger.LogType.LT_DEBUG, ("[SendTo:" + getSocketChannelAddr(socketChannel) + "](" + new String(writeByte) + ")"));
		transceiveLogger.log(String.format("[%d]snd(%s:%04d)=(", System.currentTimeMillis(), recvAddr, writeByte.length), new String(writeByte), ")");
		
		return true;
	}
	
	// ���� ����ä�� �ε��� ��ȯ ��, ä�� �ε��� ����
	private int updateCurChannelIndex() {
		int curChannelIndexCopy = 0;
		
		if (serverList == null || serverList.size() == 0) {
			curChannelIndex = 0;
		}
		else {
			curChannelIndexCopy = curChannelIndex;
			curChannelIndex = ++curChannelIndex % serverList.size();
		}
		
		return curChannelIndexCopy;
	}
	
	// �ش� ������ InetSocketAddress ��ȯ
	private InetSocketAddress getSocketChannelAddr(SocketChannel socketChannel) {
		InetSocketAddress rtAddr = null;
		
		try {
			rtAddr = (InetSocketAddress)socketChannel.getRemoteAddress();
		}
		catch (IOException ioe) {
			Logger.logln(Logger.LogType.LT_ERR, ioe);
		}
		finally {
			return rtAddr;
		}
	}
}