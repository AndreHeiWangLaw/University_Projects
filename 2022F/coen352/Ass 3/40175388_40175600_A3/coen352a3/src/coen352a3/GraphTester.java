/** Source code example for "A Practical Introduction to Data
Structures and Algorithm Analysis, 3rd Edition (Java)" 
by Clifford A. Shaffer
Copyright 2008-2011 by Clifford A. Shaffer
*/

package coen352a3;

import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import java.io.*;
import java.util.*;

public class GraphTester {
	
	static final int UNVISITED = 0;
	static final int VISITED = 1;
	static StringBuffer out;

	// PDF page 211
	@BeforeEach
	public void strBuffer(){
		StringBuffer out = new StringBuffer(100);
	}
	
	// PDF page 503
	static void concom(Graph G) {
		int i;
		for (i=0; i<G.n(); i++) // For n vertices in graph
			G.setMark(i, 0); // Vertex i in no component
		int comp = 1; // Current component
		for (i=0; i<G.n(); i++)
			if (G.getMark(i) == 0) // Start a new component
				DFS_component(G, i, comp++);
		for (i=0; i<G.n(); i++)
			out.append(i + " " + G.getMark(i) + " ");
	}
	
	// PDF page 504
	static void DFS_component(Graph G, int v, int comp) {
		G.setMark(v, comp);
		for (int w = G.first(v); w < G.n(); w = G.next(v, w))
			if (G.getMark(w) == 0)
				DFS_component(G, w, comp);
	}

	// PDF page 402
	static void DFS(Graph G, int v) { // Depth first search
		PreVisit(G, v); // Take appropriate action
		G.setMark(v, VISITED);
		for (int w = G.first(v); w < G.n() ; w = G.next(v, w))
			if (G.getMark(w) == UNVISITED)
				DFS(G, w);
		PostVisit(G, v); // Take appropriate action
	}
	
	// PDF page 402, similar to pre-order tree traversal
	static void PreVisit(Graph G, int v) {
		out.append(v + "pre "); //subject to the application
	}
	
	// PDF page 402, similar to post-order tree traversal
	static void PostVisit(Graph G, int v) {
		out.append(v + "pos "); // subject to the application 
	}
	@SuppressWarnings("null")
	static Graph createGraph(BufferedReader file, Graph G) throws IOException {
		String line = null;
		StringTokenizer token;
		boolean undirected = false;
		int i, weight, v1_int, v2_int; 
		String v1, v2; 
		
		assert (line = file.readLine()) != null :
		         "Unable to read number of vertices";
		while(line.charAt(0) == '#')
			assert (line = file.readLine()) != null : "Unable to read number of vertices";
		token = new StringTokenizer(line);
		  
		int n = Integer.parseInt(token.nextToken());
		G.Init(n);
		  
		for (i=0; i<n; i++)
		    G.setMark(i, UNVISITED);
		  
		assert (line = file.readLine()) != null : "Unable to read graph type";
		  
		if (line.charAt(0) == 'U')
			undirected = true;
		else if (line.charAt(0) == 'D')
			undirected = false;
		else 
			assert false : "Bad graph type: " + line;
		
		// Read in edges
		while((line = file.readLine()) != null) {
			token = new StringTokenizer(line);
		    v1 = token.nextToken();
		    v2 = token.nextToken();
		    if (token.hasMoreTokens())
		    	weight = Integer.parseInt(token.nextToken());
		    else // No weight given -- set at 1
		    	weight = 1;
		    v1_int = G.getIndex(v1); 
		    v2_int = G.getIndex(v2); 
		    G.setEdge(v1_int, v2_int, weight);
		    if (undirected) // Put in edge in other direction
		    	G.setEdge(v2_int, v1_int, weight);
		    }
		return G;
	}
	
	@Test
	public void testisPrerequisite() throws IOException{
		BufferedReader buffRead;
		buffRead = new BufferedReader(new InputStreamReader(new FileInputStream("coen_course.gph")));
		Graph graph = new Graphm();
		createGraph(buffRead, graph);
		
//		// isPrerequisite returns true tests
//		assertEquals(true,graph.isPrerequisite("COEN231","COEN313"), "Test failed");
//		assertEquals(true,graph.isPrerequisite("COEN346","COEN320"), "Test failed");
//		assertEquals(true,graph.isPrerequisite("COEN390","COEN490"), "Test failed");
//		assertEquals(true,graph.isPrerequisite("COEN244","COEN433"), "Test failed");
//		
//		// isPrerequisite returns false tests
//		assertEquals(false,graph.isPrerequisite("COEN352","COEN424"), "Test failed");
//		assertEquals(false,graph.isPrerequisite("COEN366","COEN212"), "Test failed");
//		assertEquals(false,graph.isPrerequisite("COEN390","COEN424"), "Test failed");
//		assertEquals(false,graph.isPrerequisite("COEN490","COEN317"), "Test failed");
		}

	@Test
	public void testGetPrerequisitePath() throws IOException{
		BufferedReader buffRead;
		buffRead = new BufferedReader(new InputStreamReader(new FileInputStream("coen_course.gph")));
		Graph graph = new Graphm();
		createGraph(buffRead, graph);
		
		// getPrerequisitePath returns the path tests
		assertEquals("COEN212 ",graph.getPrerequisitePath("COEN212"),"Test failed");
		assertEquals("COEN243 COEN244 ",graph.getPrerequisitePath("COEN244"),"Test failed");
		assertEquals("COEN231 COEN212 COEN313 ",graph.getPrerequisitePath("COEN313"),"Test failed");
		assertEquals("COEN212 COEN243 COEN244 COEN231 COEN352 COEN311 COEN346 ",graph.getPrerequisitePath("COEN346"),"Test failed");
		assertEquals("COEN212 COEN243 COEN244 COEN231 ENGR290 COEN352 COEN311 COEN390 ENGR371 ENGR301 SOEN341 COEN490 ",graph.getPrerequisitePath("COEN490"),"Test failed");
		}
}