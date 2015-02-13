package org.thunderatz.tiago.thundertrekking;

import android.app.Activity;
import android.widget.TextView;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class ServerThread extends Thread {
    int port;
    String id;
    private DatagramSocket socket;
    private TextView log;
    private InetAddress client_addr;
    private int client_port = 0;
    private ListenerRegisterer register;

    ServerThread(TextView t, int target_port, String my_id, ListenerRegisterer r) {
        log = t;
        port = target_port;
        id = my_id;
        register = r;
    }

    @Override
    public void run() {
        try {
            socket = new DatagramSocket(port);
            log.append(id + ": porta " + Integer.toString(port) + "\n");

            // Receber pacote de tamanho 0 (usado apenas para identificar endereço e porta para respostas)
            while (socket != null) {
                DatagramPacket packet = new DatagramPacket(null, 0);
                socket.receive(packet);
                client_addr = packet.getAddress();
                client_port = packet.getPort();
                log.append(id + " (" + Integer.toString(port) + "): accept " + client_addr.getHostName() + " " + Integer.toString(client_port) + "\n");
            }
        } catch (IOException e) {
            log.append(e.toString() + "\n");
            client_port = 0;
        }
    }

    public void send(byte[] data) {
        if (client_port == 0) {
            log.append(id + " - send: Sem clientes\n");
            return;
        }

        DatagramPacket packet;
        packet = new DatagramPacket(data, data.length, client_addr, client_port);
        try {
            socket.send(packet);
        } catch (IOException e) {
            log.append(e.toString() + "\n");
            client_port = 0;
        }
    }

    public void recv(byte[] data) {
        if (client_port == 0) {
            log.append(id + " - recv: Sem clientes\n");
            return;
        }

        DatagramPacket packet;
        packet = new DatagramPacket(data, data.length, client_addr, client_port);
        try {
            socket.receive(packet);
        } catch (IOException e) {
            log.append(e.toString() + "\n");
            client_port = 0;
        }
    }

    public void close() {
        socket.close();
        socket = null;
        client_port = 0;
    }
}
