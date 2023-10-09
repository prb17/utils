# Builder Pattern

## Intent
The builder pattern is meant to separate the construction of a complex object from its representation so that the same construction process can create different representations

## Aplicability
Use the Builder pattern when:
- the algorithm for creating a complex object should be independent of the parts that make up the object and how they're assembled
- the construction process must allow different representations for the object that's constructed

## Structure
```mermaid
classDiagram
director *--> builder 
builder <|-- concrete_builder
concrete_builder --> product

class director {
    construct()
}
class builder {
    build_part()
}
class concrete_builder {
    build_part()
    get_result()
}
```

## Collaborations
```mermaid
sequenceDiagram
participant client
participant director
participant concrete_builder

client-->>concrete_builder: new concrete_builder
activate concrete_builder
deactivate concrete_builder

client-->>director: new director(concrete_builder)
activate director
deactivate director

client->>director: construct()
activate director
director->>concrete_builder: build_partA()
activate concrete_builder
deactivate concrete_builder

director->>concrete_builder: build_partB()
activate concrete_builder
deactivate concrete_builder

director->>concrete_builder: build_partC()
activate concrete_builder
deactivate concrete_builder

deactivate director

client->>concrete_builder: get_result()



```