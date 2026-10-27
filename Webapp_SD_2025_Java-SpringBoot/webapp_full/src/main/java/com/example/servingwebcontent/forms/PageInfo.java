package com.example.servingwebcontent.forms;
import java.io.Serializable;

public class PageInfo implements Serializable {
    private final String title;
    private final String bodyText;

    public PageInfo(String title, String bodyText) {
        this.title = title;
        this.bodyText = bodyText;
    }

    public String getTitle() {
        return title;
    }

    public String getBodyText() {
        return bodyText;
    }
}
