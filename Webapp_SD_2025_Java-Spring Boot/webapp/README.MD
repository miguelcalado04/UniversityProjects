*Googol — Motor de Pesquisa de Páginas Web*

Projeto desenvolvido no âmbito da unidade curricular Sistemas Distribuídos (2024/25).

#Descrição

O Googol é um motor de pesquisa de páginas Web inspirado em sistemas como o Google. O sistema permite a indexação automática de páginas Web e a realização de pesquisas sobre o conteúdo indexado.

O sistema suporta:

Indexação automática de páginas Web (Web Crawler)
Pesquisa de termos em páginas indexadas
Ordenação de resultados por relevância (baseada em backlinks)
Arquitetura distribuída com RPC/RMI

Adicionalmente, permite a submissão manual de URLs para indexação e a exploração recursiva de ligações entre páginas.

#Arquitetura do Sistema

O sistema é composto por quatro principais componentes distribuídos:

Downloaders (Web Crawlers): Responsáveis por obter páginas Web, extrair conteúdo HTML e enviar a informação para indexação.
Index Storage Barrels: Servidores replicados responsáveis pelo armazenamento do índice invertido e dos metadados das páginas.
RPC/RMI Gateway: Interface intermédia entre os clientes e os Storage Barrels, responsável pelo encaminhamento de pedidos.
RPC/RMI Client: Interface utilizada pelos utilizadores para submeter URLs e realizar pesquisas.

A comunicação entre os componentes é realizada através de RPC/RMI, garantindo a distribuição e desacoplamento do sistema.

#Funcionalidades
Indexação de URLs submetidos pelo utilizador
Web crawling automático com exploração de ligações
Indexação de palavras com ligação aos URLs correspondentes
Pesquisa de páginas que contenham um conjunto de termos definidos pelo utilizador
Ordenação de resultados por número de backlinks
Sistema distribuído com replicação de servidores
