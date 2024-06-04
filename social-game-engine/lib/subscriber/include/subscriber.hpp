template<typename Subject, typename Tag>
class Subscriber{
    public:
        // The "tag" will be an enum class in the concrete Publisher class, representing the property for which we want to notify about changes.
        virtual void Update(Subject const& subject, Tag property) = 0;
        virtual ~Subscriber() = default;
};