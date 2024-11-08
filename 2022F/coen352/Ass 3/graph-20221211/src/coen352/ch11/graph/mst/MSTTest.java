package coen352.ch11.graph.mst;
import static org.junit.jupiter.api.Assertions.*;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class MSTTest {


		static final int UNVISITED = 0;
		static final int VISITED = 1;

		// Create a graph from file
		static Graph createGraph(BufferedReader file, Graph G) 
		throws IOException {
		  String line = null;
		  StringTokenizer token;
		  boolean undirected = false;
		  int i, v1, v2, weight;

		  assert (line = file.readLine()) != null :
		         "Unable to read number of vertices";
		  while(line.charAt(0) == '#')
		  assert (line = file.readLine()) != null :
		         "Unable to read number of vertices";
		  token = new StringTokenizer(line);
		  int n = Integer.parseInt(token.nextToken());
		  G.Init(n);
		  for (i=0; i<n; i++)
		    G.setMark(i, UNVISITED);
		  assert (line = file.readLine()) != null :
		         "Unable to read graph type";
		  if (line.charAt(0) == 'U')
		    undirected = true;
		  else if (line.charAt(0) == 'D')
		    undirected = false;
		  else assert false : "Bad graph type: " + line;

		  // Read in edges
		  while((line = file.readLine()) != null) {
			token = new StringTokenizer(line);
		    v1 = Integer.parseInt(token.nextToken());
		    v2 = Integer.parseInt(token.nextToken());
		    if (token.hasMoreTokens())
		      weight = Integer.parseInt(token.nextToken());
		    else // No weight given -- set at 1
		      weight = 1;
		    G.setEdge(v1, v2, weight);
		    if (undirected) // Put in edge in other direction
		      G.setEdge(v2, v1, weight);
		  }
		  return G;
		}

		static void Gprint(Graph G, StringBuffer out) {
		  int i, j;

		  for (i=0; i<G.n(); i++) {
		    for(j=0; j<G.n(); j++)
		      if (G.weight(i, j) == Integer.MAX_VALUE)
		        out.append("0 ");
		      else
		        out.append(G.weight(i, j) + " ");
		  }
		}


		static int[] D = new int[100]; //distance array 
		static StringBuffer out,path;
		

	
		
		
		static int minVertex(Graph G, int[] D) {
		  int v = 0;  // Initialize v to any unvisited vertex;
		  for (int i=0; i<G.n(); i++)
		    if (G.getMark(i) == UNVISITED) { v = i; break; }
		  for (int i=0; i<G.n(); i++)  // Now find smallest value
		    if ((G.getMark(i) == UNVISITED) && (D[i] < D[v]))
		      v = i;
		  return v;
		}
		
		
		// Compute a minimal-cost spanning tree
		static void Prim(Graph G, int s, int[] D, int[] V) {
		  for (int i=0; i<G.n(); i++)   // Initialize
		  D[i] = Integer.MAX_VALUE;
		  D[s] = 0;
		  for (int i=0; i<G.n(); i++) { // Process the vertices
		    int v = minVertex(G, D);
		    G.setMark(v, VISITED);
		    
		    if (v != s) AddEdgetoMST(V[v], v);
		    if (D[v] == Integer.MAX_VALUE) return; // Unreachable
		    for (int w = G.first(v); w < G.n(); w = G.next(v, w))
		      if (D[w] > G.weight(v, w)) {
		        D[w] = G.weight(v, w);
		        V[w] = v;
		      }
		  }
		}
		
		
		static void AddEdgetoMST(int v1, int v2) {
			path.append("(" + v1 + "," + v2 + ") "); 
		}
		
		static void PrimMinHeap(Graph G, int s, int[] D, int[] V) {
			  int v;                               // The current vertex
			  DijkElem[] E = new DijkElem[G.e()];  // Heap for edges
			  E[0] = new DijkElem(s, 0);           // Initial vertex
			  MinHeap<DijkElem> H = new MinHeap<DijkElem>(E, 1, G.e());
			  for (int i=0; i<G.n(); i++)          // Initialize
			    D[i] = Integer.MAX_VALUE;          //   distances
			  D[s] = 0;
			  for (int i=0; i<G.n(); i++) {        // Now, get distances
			    do { v = (H.removemin()).vertex(); } // Get position
			      while (G.getMark(v) == VISITED);
			    G.setMark(v, VISITED);
			    if (v != s) AddEdgetoMST(V[v], v); // Add edge to MST
			    if (D[v] == Integer.MAX_VALUE) return; // Unreachable
			    for (int w = G.first(v); w < G.n(); w = G.next(v, w))
			      if (D[w] > G.weight(v, w)) {     // Update D
			        D[w] = G.weight(v, w);
			        V[w] = v;                      // Where it came from
			        H.insert(new DijkElem(w, D[w]));
			      }
			  }
			}
		
		  /**
		   * This method is automatically called once before each test case
		   * method, so that all the variables are cleanly initialized for
		   * each test.
		   */
		 @BeforeEach
		  public void setUp()
		  {
		    out = new StringBuffer(100);
		    path = new StringBuffer(1000);
		  }

		@Test
		  public void testPrim()
		  throws IOException {
		    BufferedReader f;
		    f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-final.gph")));
		    Graph G = new Graphm();
		    createGraph(f, G);
		    int[] V = new int[G.n()];     // V[i] stores closest vertex to i
		    Prim(G, 3, D, V);
		    for (int i=0; i< G.n(); i++) out.append(V[i] + " ");
		    //assertEquals(out.toString(), "0 2 3 2 5 4 ");
		    System.out.println("path prim=" + path);
		    System.out.println("v[i]: "+ out);
		    
		    for (int i=0; i< G.n(); i++)  
		    	System.out.println("D[]: "+ i + " " + D[i]);
		  }
		 
		@Test
		 public void testPrimMinHeap()
				  throws IOException {
				    BufferedReader f;
				    f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-final.gph")));
				    Graph G = new Graphm();
				    createGraph(f, G);
				    int[] V = new int[G.n()];     // V[i] stores closest vertex to i
				    PrimMinHeap(G, 3, D, V);
				    for (int i=0; i< G.n(); i++) out.append(V[i] + " ");
				    //assertEquals(out.toString(), "0 2 3 2 5 4 ");
				    System.out.println("path heap=" + path);
				    
				    System.out.println("v[i]: "+ out);
				    
				    for (int i=0; i< G.n(); i++)  
				    	System.out.println("D[]: "+ i + " " + D[i]);
				    
				    
				   
		}
		 
		 
		// @Test
		  public void testExam()
		  throws IOException {
		    BufferedReader f;
		    f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-mst-final.gph")));
		    Graph G = new Graphm();
		    createGraph(f, G);
		    int[] V = new int[G.n()];     // V[i] stores closest vertex to i
		    Prim(G, 1, D, V);
		    for (int i=0; i< G.n(); i++) out.append(V[i] + " ");
		    //System.out.println("path =" + path);
		    System.out.println("v[i]: "+ out);
		    System.out.println("v[i]: "+ out);
		   
		  }
		 //assertEquals(out.toString(), "0 2 3 2 5 4 ");
		   
		 

}
