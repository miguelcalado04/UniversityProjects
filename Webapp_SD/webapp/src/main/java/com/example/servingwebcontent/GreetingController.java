package com.example.servingwebcontent;

import com.example.servingwebcontent.forms.Index; 
import com.example.servingwebcontent.forms.SearchClient; 
import java.rmi.RemoteException;
import java.io.Serializable;
import java.rmi.NotBoundException; 
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.stream.Collectors;

import javax.naming.directory.SearchResult;

import java.util.ArrayList;
import java.util.Arrays;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.simp.SimpMessagingTemplate;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.example.servingwebcontent.forms.Callback;
import com.example.servingwebcontent.forms.Gateway;

import java.util.HashMap;

@Controller
public class GreetingController extends UnicastRemoteObject implements Callback {


    protected GreetingController() throws RemoteException {
        super();
        //TODO Auto-generated constructor stub
    }

    @Autowired
    private Gateway gateway;  

    @Autowired
    private HackerNewsService hackerNewsService;

    //@Autowired
    //private OpenAiService openAiService;

    //@Autowired
    //private OpenRouterService openRouterService;


    
    // ------------------ ADICIONAR LINK ------------------------

    @GetMapping("/")
    public String home() {
        return "home";
    }


    @GetMapping("/add-link")
    public String showForm( Model model) {
        model.addAttribute("message", "Link adicionado com sucesso!");  
        return "add-link";
    }

    @PostMapping("/add-link")
    public String submitLink(@RequestParam String link, Model model) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 8190); // Conectando ao Gateway
            Index gatewayRMI = (Index) registry.lookup("Gateway");

            SearchClient client = new SearchClient();

            gatewayRMI.registerCallback(client);

            gatewayRMI.putNew(link);
            
            model.addAttribute("message", "Link adicionado com sucesso!");  

        } catch (RemoteException e) {
            e.printStackTrace();  
            model.addAttribute("message", "Erro ao adicionar o link.");
            return "error";  
        } catch (NotBoundException e) {
            e.printStackTrace(); 
            model.addAttribute("message", "Erro: O objeto Gateway não está registrado no RMI Registry.");
            return "error"; 
        }
        return "add-link"; 
    }


    @Override
    public void onSearchResults(List<Map<String, String>> results) throws RemoteException {
        System.out.println("Resultados da pesquisa: " + results);
    }


        

    //----------------- PESQUISAR PALAVRAS ------------------------
   
    @GetMapping("/search")
    public String search(@RequestParam(value = "keywords", required = false) String keywords, 
                        @RequestParam(defaultValue = "1") Integer page,
                        Model model) {
        if (keywords == null || keywords.isEmpty()) {
            return "search";
        }

        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 8190);
            Index gateway = (Index) registry.lookup("Gateway");

            SearchClient client = new SearchClient();
            gateway.registerCallback(client);

            List<String> palavras = Arrays.asList(keywords.split("\\s+"));
            gateway.searchWordsWithDetails(palavras, page);

            List<Map<String, String>> resultados = client.getSearchResults();

            /* 
            //----------- OpenAI --------------
            StringBuilder context = new StringBuilder("Analisa os seguintes resultados de pesquisa:\n\n");
            for (Map<String, String> resultado : resultados) {
                String title = resultado.getOrDefault("title", "");
                String link = resultado.getOrDefault("link", "");
                context.append("- ").append(title).append(" (").append(link).append(")\n");
            }
            String iaAnalysis = openAiService.generateContextualAnalysis(context.toString());
            //---------------------------------
            */
            //----------- TENTATIVAS --------------
            /*StringBuilder context = new StringBuilder("Analisa os seguintes resultados de pesquisa:\n\n");
            for (Map<String, String> resultado : resultados) {
                String title = resultado.getOrDefault("title", "");
                String link = resultado.getOrDefault("link", "");
                context.append("- ").append(title).append(" (").append(link).append(")\n");
            }
            String iaAnalysis = openRouterService.generateContextualAnalysis(context.toString());*/
            //---------------------------------

            model.addAttribute("results", resultados);
            model.addAttribute("keywords", keywords);
            model.addAttribute("page", page);
            //model.addAttribute("iaAnalysis", iaAnalysis); // ✅ Passa a análise para o template

            return "search";
        } catch (Exception e) {
            e.printStackTrace();
            model.addAttribute("message", "Erro ao pesquisar.");
            return "error";
        }
    }



    // --------------pesquisa links---------------//

    @GetMapping("/searchlinks")
    public String searchLinks(@RequestParam(value = "keywords", required = false) String keywords, @RequestParam(defaultValue = "1") Integer page, Model model) {
        if (keywords == null || keywords.isEmpty()) {
            return "searchlinks";
 
        }

        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 8190);
            Index gateway = (Index) registry.lookup("Gateway");

            SearchClient client = new SearchClient();
            gateway.registerCallback(client);

            String link = keywords;

            
            gateway.searchLinks(link, page);  

            List<Map<String, String>> resultados = client.getSearchResults();

            model.addAttribute("results", resultados);
            model.addAttribute("keywords", keywords);
            model.addAttribute("page", page);

            return "searchlinks";

        } catch (Exception e) {
            e.printStackTrace();
            model.addAttribute("message", "Erro ao pesquisar links.");
            return "error";
        }
    }

    //------------------ HACKER NEWS ------------------ ~


    @GetMapping("/hacker-index")
    public String showHackerForm() {
        return "hacker-index";
    }

    @PostMapping("/hacker-index")
    public String indexHackerNews(@RequestParam String keywords, Model model) {
        List<String> keywordsList = Arrays.asList(keywords.split("\\s+"));
        List<String> urls = hackerNewsService.getTopStoryUrlsWithKeywords(keywordsList);

        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 8190);
            Index gateway = (Index) registry.lookup("Gateway");

            for (String url : urls) {
                gateway.putNew(url);
            }

            model.addAttribute("message", "Foram indexados " + urls.size() + " links do Hacker News com sucesso!");
        } catch (Exception e) {
            model.addAttribute("message", "Erro ao indexar URLs do Hacker News.");
            e.printStackTrace();
        }

        return "hacker-index";
    }


    // ----------------- OpenAI -----------------

    


    // ---------------- TOP 10 PESQUISAS ----------------

    /* 

    @GetMapping("/top10")
    public String getTop10Searches(Model model) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost", 8190);
            Index gateway = (Index) registry.lookup("Gateway");

            SearchClient client = new SearchClient();
            gateway.registerCallback(client);

            System.out.println("Top 10 Pesquisas:");

            // Obtém as estatísticas do gateway
            List<Map<String, Object>> topSearchesList = gateway.getSystemStats(); 

            // A lista de resultados que vamos preencher com os 10 mais pesquisados
            List<Map<String, Object>> top10Searches = new ArrayList<>();

            // Itera sobre a lista de mapas (topSearchesList)
            for (Map<String, Object> topSearches : topSearchesList) {
                // Para cada mapa, obtém a lista associada à chave "top10Searches"
                List<Map<String, Object>> searches = (List<Map<String, Object>>) topSearches.get("top10Searches");

                // Ordena os itens dessa lista com base na contagem (supõe-se que cada item tem uma chave 'count')
                List<Map<String, Object>> sortedSearches = searches.stream()
                    .sorted((entry1, entry2) -> {
                        Integer count1 = (Integer) entry1.get("count");
                        Integer count2 = (Integer) entry2.get("count");
                        return count2.compareTo(count1); // Ordena em ordem decrescente
                    })
                    .limit(10) // Limita para os 10 primeiros
                    .collect(Collectors.toList());

                // Adiciona os resultados ordenados à lista final
                top10Searches.addAll(sortedSearches);
            }

            // Passa os resultados para o modelo
            model.addAttribute("top10", top10Searches);
            
            return "top10"; // Página para exibir os resultados
        } catch (Exception e) {
            e.printStackTrace();
            model.addAttribute("message", "Erro ao buscar o top 10.");
            return "error"; // Página de erro em caso de falha
        }
    }

    */

    // Método para enviar as top 10 pesquisas para os clientes conectados via WebSocket

    @GetMapping("/top10")
    public String getTop10Searches(Model model) {
        try {

           

            // Envia a lista para os clientes via WebSocket
            

            //model.addAttribute("top10", top10Searches);
            return "top10"; // Página para exibir os resultados
        } catch (Exception e) {
            e.printStackTrace();
            model.addAttribute("message", "Erro ao buscar o top 10.");
            return "error";
        }
    }



    @Override
    public void onSearchComplete(List<Map<String, String>> results) throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'onSearchComplete'");
    }

   
    @Override
    public void tops(Map<String, Object> results) throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'tops'");
    }

    @Override
    public void barr(List<Integer> results) throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'barr'");
    }

    @Override
    public void onError(String message) throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'onError'");
    }

    @Override
    public Map<String, Object> getSystemStats() throws RemoteException {
        // TODO Auto-generated method stub
        throw new UnsupportedOperationException("Unimplemented method 'getSystemStats'");
    }
}
