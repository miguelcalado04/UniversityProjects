package com.example.servingwebcontent;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;
import java.util.*;

@Service
public class HackerNewsService {
    private final RestTemplate restTemplate = new RestTemplate();
    private final String BASE_URL = "https://hacker-news.firebaseio.com/v0/";

    public List<String> getTopStoryUrlsWithKeywords(List<String> keywords) {
        List<String> matchedUrls = new ArrayList<>();
        Integer[] topStoryIds = restTemplate.getForObject(BASE_URL + "topstories.json", Integer[].class);

        for (int i = 0; i < Math.min(topStoryIds.length, 50); i++) {
            Map story = restTemplate.getForObject(BASE_URL + "item/" + topStoryIds[i] + ".json", Map.class);
            if (story != null && story.get("title") != null) {
                String title = story.get("title").toString().toLowerCase();
                String text = story.get("text") != null ? story.get("text").toString().toLowerCase() : "";

                boolean containsKeyword = keywords.stream().anyMatch(k -> title.contains(k.toLowerCase()) || text.contains(k.toLowerCase()));

                if (containsKeyword && story.get("url") != null) {
                    matchedUrls.add(story.get("url").toString());
                }
            }
        }
        return matchedUrls;
    }
}
