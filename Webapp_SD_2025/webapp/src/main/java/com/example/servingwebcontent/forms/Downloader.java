package com.example.servingwebcontent.forms;

import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.*;
import java.io.IOException;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

public class Downloader {

    private static Map<Integer, Index> conectarBarrels(Map<Integer, Index> barrels) {
        for (int porta = 0; porta < 9999; porta++) {
            if (!barrels.containsKey(porta)) {
                try {
                    Registry registry = LocateRegistry.getRegistry("localhost", porta);
                    Index barrel = (Index) registry.lookup("StorageBarrel");
                    barrels.put(porta, barrel);
                    System.out.println("Barrel conectado na porta " + porta);
                } catch (Exception ignored) {}
            }
        }
        return barrels;
    }

    private static void enviarLogRMI(Map<Integer, Index> barrels, String message) {
        List<Integer> portsToRemove = new ArrayList<>();
        for (Map.Entry<Integer, Index> entry : barrels.entrySet()) {
            int port = entry.getKey();
            Index barrel = entry.getValue();
            try {
                barrel.logMessage(message);
            } catch (RemoteException e) {
                System.out.println("Erro ao enviar log para barril na porta " + port + ". Marcando para remoção.");
                portsToRemove.add(port);
            }
        }
        for (int port : portsToRemove) {
            barrels.remove(port);
        }
    }

    public static void main(String[] args) {
        Map<Integer, Index> barrels = new HashMap<>();
        barrels = conectarBarrels(barrels);
        List<Integer> barrelPorts = new ArrayList<>(barrels.keySet());
        int barrelIndex = 0;

        while (true) {
            try {
                barrels = conectarBarrels(barrels);
                barrelPorts = new ArrayList<>(barrels.keySet());

                if (barrels.isEmpty()) {
                    System.out.println("Nenhum barril disponível. Tentando novamente em 5 segundos...");
                    Thread.sleep(5000);
                    continue;
                }

                if (barrelIndex >= barrelPorts.size()) {
                    barrelIndex = 0;
                }

                int porta = barrelPorts.get(barrelIndex);
                Index server = barrels.get(porta);
                String url;
                try {
                    url = server.takeNext();
                } catch (Exception e) {
                    System.out.println("Erro ao conectar ao barril na porta " + porta + ". Removendo da lista...");
                    barrels.remove(porta);
                    barrelPorts.remove(barrelIndex);
                    continue;
                }

                if (url == null) {
                    Thread.sleep(1000);
                    barrelIndex = (barrelIndex + 1) % barrelPorts.size();
                    continue;
                }

                System.out.println("URL:" + url);
                server.markVisited(url);

                try {
                    Document doc = Jsoup.connect(url).get();
                    String title = doc.title();
                    String bodyText = doc.body().text();

                   
                    List<Integer> failedPorts = new ArrayList<>();
                    for (Map.Entry<Integer, Index> entry : barrels.entrySet()) {
                        try {
                            entry.getValue().savePageData(url, title, bodyText);
                        } catch (RemoteException e) {
                            System.out.println("Falha ao enviar savePageData para porta " + entry.getKey());
                            failedPorts.add(entry.getKey());
                        }
                    }
                    for (int failedPort : failedPorts) {
                        barrels.remove(failedPort);
                    }
                    //System.out.println("Título: " + title);
                    //System.out.println("Texto do corpo: " + bodyText);

                    // Indexar palavras
                    StringBuilder indexData = new StringBuilder("URL: " + url + " | Palavras: ");
                    for (String word : bodyText.split("\\W+")) {
                        if (!word.isBlank()) {
                            server.addToIndex(word.toLowerCase(), url);
                            indexData.append(word.toLowerCase()).append(", ");
                        }
                    }

                    // Links encontrados
                    Elements links = doc.select("a[href]");
                    StringBuilder linksData = new StringBuilder("Links encontrados: ");
                    for (Element link : links) {
                        String linkHref = link.attr("abs:href");
                        System.out.println("URL:" + linkHref);
                        server.putNew(linkHref);
                        server.addLink(url, linkHref);
                        linksData.append(linkHref).append(", ");
                    }

                    // Cortar trailing vírgulas
                    if (indexData.length() > 2) indexData.setLength(indexData.length() - 2);
                    if (linksData.length() > 2) linksData.setLength(linksData.length() - 2);

                    enviarLogRMI(barrels, indexData.toString());
                    enviarLogRMI(barrels, linksData.toString());

                } catch (IOException e) {
                    System.out.println("Erro ao processar URL, ignorando...");
                }

                barrelIndex = (barrelIndex + 1) % barrelPorts.size();
            } catch (Exception e) {
                System.out.println("Erro geral: " + e.getMessage());
            }
        }
    }
}
