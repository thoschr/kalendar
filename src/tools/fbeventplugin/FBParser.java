package fbeventplugin;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.time.ZonedDateTime;
import java.util.Date;

public class FBParser {
	
	private static FBEvent createEvent(String html) {
		String title;
		String description;
		String place;
		ZonedDateTime start = null;
		ZonedDateTime end = null;
		
		int pos = html.indexOf("pageTitle") + 11;
		title = html.substring(pos, html.indexOf("</title", pos));
		try {
			pos = html.indexOf("timeRowTitle", pos) + 29;
			start = ZonedDateTime.parse(html.substring(pos, html.indexOf(" to", pos)));
			pos = html.indexOf("to ", pos) + 3;
			end = ZonedDateTime.parse(html.substring(pos, html.indexOf("\">", pos)));
		} catch (Exception e) {
			System.err.println("Exception occurred in date parsing: "+e.getMessage());
			return null;
		}
		pos = html.indexOf("fcg\">", html.indexOf("event-permalink-location", pos)) + 5;
		place = html.substring(pos, html.indexOf("</div", pos));
		pos = html.indexOf("event-permalink-details", pos) + 25;
		html = html.replace("<span class=\"text_exposed_hide\">...</span>", "");
		String rawDescription = html.substring(pos, html.indexOf("</span", pos) - 2);
		description = rawDescription.replaceAll("&#039;", "'")
									.replaceAll("&#064;", "@")
									.replaceAll("<br[^a-z]*>", "\n")
									.replaceAll("(?s)<[^>]*>(\\s*<[^>]*>)*", " ");
		return new FBEvent(title, description, place, start, end);
	}
		
	public boolean addEvent(String uri) {
		try {
			URL obj = new URL(uri);
			HttpURLConnection con = (HttpURLConnection) obj.openConnection();
			con.setRequestMethod("GET");
			int responseCode = con.getResponseCode();
			if (responseCode == HttpURLConnection.HTTP_OK) { // success
				BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
				String inputLine;
				StringBuffer response = new StringBuffer();
	
				while ((inputLine = in.readLine()) != null) {
					response.append(inputLine);
				}
				in.close();
	
				FBEvent fbe = createEvent(response.toString());
				if (fbe == null)
					return false;
				else {
					String[] command = new String[]{"kalendar", "-a", fbe.getTitle() + "##" + fbe.getDescription() + "##"
							+ fbe.getPlace() + "##facebook##" + fbe.getStart().toEpochSecond() + "##"
							+ fbe.getEnd().toEpochSecond()};
					String output = "";
					Process p;
					p = Runtime.getRuntime().exec(command);
					p.waitFor();
					BufferedReader reader = new BufferedReader(new InputStreamReader(p.getErrorStream()));
					String line = "";
					while ((line = reader.readLine()) != null) {
						output += line + "\n";
					}
					return true;
					/* if (output == "") return true; */
				}
			} else {
				System.err.println("GET request not worked");
			}
		} catch(Exception e) {
			System.err.println("Exception: "+e.getMessage());
		}
		return false;
	}
	
}
