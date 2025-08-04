
Relatório Meta2

•	Arquitetura do projeto Web
O nosso projeto foi feito em java, nesta mente a classe que principal que comunica com o gateway e com o cliente web foi GreetingController. Nesta meta já tínhamos as funções principais já feitas da ultima meta tínhamos de criar o frontend, as principais componentes para o fazermos foram as seguintes:
- O servidor Web desenvolvido com Spring Boot, este atua simultaneamente como cliente do servidor RMI e servidos HTTP para os utilizadores Web, este recebe os pedidos HTTP vindos do utilizadores e reencaminha-os para a gateway gateway via RMI e retorna os resultados por páginas HTML.
- O cliente Web também é uma componente fundamental, uma vez que é aqui que os utilizadores fazem os pedidos tais como, indexar urls, pesquisar palavras, pesquisar urls, consultar as estatísticas.
A comunicação entre estas duas componentes é feita através de HTTP para os pedidos dos utilizadores e por websocket para atualizações em tempo real(top10) que vai atualizando sempre que for necessário


•	Integração do Spring Boot com o Servidor RPC/RMI da primeira meta

Para a integração do Spring Boot com o servidor da primeira meta criamos a classe Greeting Controlar que atua como ponto central entre o servidor web e o gateway. Sendo assim este gere os pedidos provenientes via http e de seguida estabelece ligação via rmi com o gateway para que com as funções que criamos na primeira meta sejam usadas através do servidor web.
A classe Greeting Controller também expõe endpoints através do @GetMapping e @PostMapping , permitindo aceder às seguintes funcionalidades:
-   /add-link – Adiciona link para ser indexado;
-   /search – Pesquisa através de palavra-chaves urls;
-   /searchlinks – Pesquisa links associados a um link chave;
-   /hacker-index – Indexação de URLs do Hacker News com base em palavras-chaves
-   /top10 - Mostar o top10 de palavras pesquisadas **
Estes endpoints servem para aceder diretamente á gateway usando rmi para fazer a conecção. É necessário assim usar o RMI registry para depois se obter uma referência remota da interface Index, que representa o objeto remoto exposto pelo gateway. Com isto depois é só invocar os métodos da gateway conforme o pedido.

** apesar do nosso top10 não estar implementado usando websockets decidimos incluir também o /top10


•	Integração de WebSockets com Spring Boot e RPC/RMI

Os WebSockets permitem que um cliente e um servidor mantenham uma linha de comunicação aberta, tornando possível o envio de mensagens em tempo real sem pedidos e respostas constantes.
No nosso projeto o websocket não está a funcionar para atualizar o top10 sem a necessidade de dar refresh na pagina web, mas o que nós fizemos foi criar uma classe WebSocketConfig com as notas @Configuration e @EnableWebSocketMessageBroker para utilizar o STOMP sobre o websocket. O STOMP descreve formatos específicos para trocar quadros de mensagens entre o cliente e o servidor.
Esta classe tem como o objetivo estabelecer o endpoint /my-websocket para os utilizadores se conectarem e receber as notificações em tempo real. 


• Integração de REST WebServices no projeto

A funcionalidade implementada foi a indexação automática de URLs provenientes do Hacker News, com base na palavras-chave introduzidas pelo utilizador.
Para tal, desenvolvemos uma nova classe : HackerNewsService, com @Service, que tem toda a lógica de comunicação com a API pública do Hacker News.
Esta classe obtem os IDs através do endpoint:https://hacker-news.firebaseio.com/v0/topstories.json
De seguida, para cada um dos 50 primeiros IDs (limitamos para não sobrecarregar o sistema), são obtidos os dados completos usando: https://hacker-news.firebaseio.com/v0/item/{id}.json
A partir desses dados, são analisados o title, text e url, e se algum dos campos contiver as palavras-chave fornecidas pelo utilizador, o link é guardado.
O controlador GreetingController é responsável por receber a palavra, invocar o HackerNewsService, e enviar os links filtrados para a gateway para serem indexados.
Exemplo de fluxo:
-Utilizador submete o formulário /hacker-index com a palavra desejada
-O Spring Boot chama HackerNewsService.getTopStoryUrlsWithKeywords(...).
-Os links retornados são enviados para a gateway
-Mensagem de sucesso é exibida ao utilizador.



TABELA DE TESTES:

Indexar novo URL introduzido por utilizador	- Pass
Indexar iterativamente ou recursivamente todos os URLs encontrados	- Pass
Pesquisar páginas que contenham um conjunto de palavras	- Pass
Páginas ordenadas por número de ligações recebidas de outras páginas - Pass
Consultar lista de páginas com ligações para uma página específica - Pass
Resultados de pesquisa paginados de 10 em 10 - Pass
Top 10 de pesquisas mais comuns atualizado em tempo real -  *Fail*
Lista de barrels ativos com respetivos tamanhos do índice e tempo de resposta - *Fail*
Indexar URLs de fonte externa (ex: HackerNews) contendo termos da pesquisa - Pass
Gerar texto contextualizado com IA (OpenAI, Gemini, etc.) - *Fail*


A estrutura do nosso código com as pastas organizadas foi incluída no zip (pasta com nome projeto)
INES NISA - MIGUEL CALADO