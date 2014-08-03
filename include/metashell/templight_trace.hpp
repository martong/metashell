#ifndef METASHELL_TEMPLIGHT_TRACE_HPP
#define METASHELL_TEMPLIGHT_TRACE_HPP

#include <string>
#include <vector>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <just/console.hpp>

#include "file_location.hpp"

namespace metashell {

enum instantiation_kind {
  template_instantiation,
  default_template_argument_instantiation,
  default_function_argument_instantiation,
  explicit_template_argument_substitution,
  deduced_template_argument_substitution,
  prior_template_argument_substitution,
  default_template_argument_checking,
  exception_spec_instantiation,
  memoization
};

class templight_trace {
public:

  static templight_trace create_from_xml(const std::string& file);

  void print_graph(std::ostream& os = std::cout) const;
  void print_graphviz(std::ostream& os = std::cout) const;

  void print_forwardtrace(const std::string& type, unsigned width) const;
  void print_full_forwardtrace(unsigned width) const;
  void print_backtrace(const std::string& type, unsigned width) const;

private:

  struct template_vertex_property_tag {
    typedef boost::vertex_property_tag kind;
  };
  struct template_edge_property_tag {
    typedef boost::edge_property_tag kind;
  };

  struct template_vertex_property {
    std::string name;
    file_location point_of_instantiation;
  };
  struct template_edge_property {
    instantiation_kind kind;
  };

  typedef boost::property<
    template_vertex_property_tag,
    template_vertex_property> vertex_property;
  typedef boost::property<
    template_edge_property_tag,
    template_edge_property> edge_property;

  typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    vertex_property,
    edge_property> graph_t;

  typedef boost::graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef boost::graph_traits<graph_t>::edge_descriptor edge_descriptor;

  typedef boost::graph_traits<graph_t>::vertex_iterator vertex_iterator;
  typedef boost::graph_traits<graph_t>::edge_iterator edge_iterator;
  typedef boost::graph_traits<graph_t>::in_edge_iterator in_edge_iterator;
  typedef boost::graph_traits<graph_t>::out_edge_iterator out_edge_iterator;

  typedef std::map<std::string, vertex_descriptor> element_vertex_map_t;

  typedef boost::property_map<
      graph_t,
      template_vertex_property_tag>::type vertex_property_map_t;

  typedef boost::property_map<
      graph_t,
      template_vertex_property_tag>::const_type const_vertex_property_map_t;

  typedef boost::property_map<
      graph_t,
      template_edge_property_tag>::type edge_property_map_t;

  typedef boost::property_map<
      graph_t,
      template_edge_property_tag>::const_type const_edge_property_map_t;

  typedef std::pair<
        std::string::const_iterator,
        std::string::const_iterator
      > string_range;

  typedef std::vector<bool> discovered_t;

  vertex_descriptor add_vertex(
      const std::string& element,
      const file_location& point_of_instantiation);

  void add_edge(
      vertex_descriptor from,
      vertex_descriptor to,
      instantiation_kind kind);

  boost::optional<vertex_descriptor> find_vertex(
      const std::string& element) const;

  std::pair<out_edge_iterator, out_edge_iterator> get_out_edges(
      vertex_descriptor v) const;
  std::pair<in_edge_iterator, in_edge_iterator> get_in_edges(
      vertex_descriptor v) const;

  vertex_descriptor get_source(edge_descriptor e) const;
  vertex_descriptor get_target(edge_descriptor e) const;

  string_range find_type_emphasize(const std::string& type) const;

  void print_trace_content(
      string_range range,
      string_range emphasize) const;

  void print_trace_graph(
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      bool print_mark) const;

  void print_trace_line(
      vertex_descriptor root_vertex,
      unsigned depth,
      const std::vector<unsigned>& depth_counter,
      const boost::optional<instantiation_kind>& kind,
      unsigned width) const;

  template<class EdgeIterator, class GetEdges, class EdgeDirection>
  void print_trace_visit(
      vertex_descriptor vertex,
      discovered_t& discovered,
      GetEdges get_edges,
      EdgeDirection edge_direction,
      unsigned width) const;

  //vertex names are currently stored reduntantly:
  // - in graph vertex_name property
  // - in element_vertex_map
  graph_t graph;

  element_vertex_map_t element_vertex_map;

  struct property_writer;
  struct is_memoziation_predicate;

  friend struct templight_trace_builder;

  const static std::vector<just::console::color> colors;
};

std::ostream& operator<<(std::ostream& os, instantiation_kind kind);
std::ostream& operator<<(std::ostream& os, const templight_trace& trace);

}

#endif