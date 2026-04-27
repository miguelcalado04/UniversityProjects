# Googol — Web Search Engine <p align="center"><img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/java/java-original.svg" width="60"/></p>

Developed as part of Distributed Systems (2024/25).

## Description

Googol is a web page search engine inspired by systems such as Google. The system allows automatic indexing of web pages and searching over the indexed content.

The system supports:

- Automatic web page indexing (Web Crawler)
- Search for terms in indexed pages
- Ranking of results by relevance (based on backlinks)
- Distributed architecture using RPC/RMI

Additionally, it allows manual submission of URLs for indexing and recursive exploration of links between pages.

## System Architecture

The system is composed of four main distributed components:

- **Downloaders (Web Crawlers):** Responsible for fetching web pages, extracting HTML content, and sending the information for indexing.
- **Index Storage Barrels:** Replicated servers responsible for storing the inverted index and page metadata.
- **RPC/RMI Gateway:** Intermediate interface between clients and the Storage Barrels, responsible for request routing.
- **RPC/RMI Client:** Interface used by users to submit URLs and perform searches.

Communication between components is carried out through RPC/RMI.

## Functionalities

- Integration with the Hacker News API to fetch and index top stories associated with the user’s search terms
- Indexing of URLs submitted by the user
- Automatic web crawling with link exploration
- Indexing of words mapped to corresponding URLs
- Searching for pages containing a set of user-defined terms
- Ranking of results by number of backlinks
- Distributed system with server replication
